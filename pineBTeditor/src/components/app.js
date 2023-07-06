import { useState, useEffect, useMemo } from 'react'
import { Button, Toast, ToastContainer, Modal, Stack } from 'react-bootstrap'
import Canvas from './canvas'
import BehaviorCard from './behavior-card'
import ConnectionLine from './connection-line'
import BehaviorDrawer from './behavior-drawer'
import BehaviorEdit from './behavior-edit'
import BlackboardDrawer from './blackboard-drawer'
import { rootBehavior } from '../models/behavior'
import { toBlueprint } from '../models/blueprint'
import * as Document from '../models/document'
import { newBlackboard } from '../models/blackboard'
import * as Editor from '../models/editor'
import bem from '../bem'

import 'bootstrap/dist/css/bootstrap.min.css'
import './app.less'

export default function App() {
    const [schemas, setSchemas] = useState([])
    const [behaviors, setBehaviors] = useState([rootBehavior()])
    const [connections, setConnections] = useState([])
    const [newConnection, setNewConnection] = useState()
    const [blackboard, setBlackboard] = useState(newBlackboard())
    const [mousePosition, setMousePosition] = useState()
    const [scrollOffset, setScrollOffset] = useState({ x: 0, y: 0 })
    const [isDragging, setDragging] = useState(false)
    const [showBehaviorDrawer, setShowBehaviorDrawer] = useState(false)
    const [showBehaviorEdit, setShowBehaviorEdit] = useState(false)
    const [showBlackboardDrawer, setShowBlackboardDrawer] = useState(false)
    const [inEditBehaviorId, setInEditBehaviorId] = useState()
    const [notifications, setNotifications] = useState([])
    const [modal, setModal] = useState(null)
    const [documentFilePath, setDocumentFilePath] = useState()
    const [savedDocumentData, setSavedDocumentData] = useState()

    const document = {
        filePath: documentFilePath,
        setFilePath: setDocumentFilePath, 
        behaviors,
        setBehaviors,
        connections,
        setConnections,
        schemas,
        setSchemas,
        blackboard,
        setBlackboard,
        savedData: savedDocumentData,
        setSavedData: setSavedDocumentData,
    }

    const documentData = useMemo(
        () => Document.toData(document), 
        [behaviors, connections]
    )

    if (!savedDocumentData) {
        setSavedDocumentData(documentData)
    }

    const isDirty = savedDocumentData != documentData

    const editor = {
        document,
        mousePosition,
        setMousePosition,
        scrollOffset,
        setScrollOffset,
        isDragging,
        setDragging,
        newConnection,
        setNewConnection,
        setShowBehaviorDrawer,
        showBehaviorEdit,
        setShowBehaviorEdit,
        inEditBehaviorId,
        setInEditBehaviorId
    }

    useEffect(() => {
        try {
            Document.loadSchemas(document)
        } catch (error) {
            console.error(error)
        }
    }, [])

    function handleBehaviorDrawerOnSelectSchema(schema) {
        const newBehavior = Document.newBehaviorFromSchema(document, schema)
        const newBehaviorAtMousePosition = {
            ...newBehavior,
            position: mousePosition
        }
        Document.addBehavior(document, newBehaviorAtMousePosition)
        if (newConnection) {
            Document.addConnection(document, { ...newConnection, to: newBehavior.id })
            Editor.endNewConnection(editor)
        }
    }

    function handleConnectionOnClick(connection) {
        Document.deleteConnection(document, connection)
    }

    function hideBehaviorDrawer() {
        Editor.endNewConnection(editor)
        setShowBehaviorDrawer(false)
    }

    function handleBehaviorCardEdit(behavior) {
        Editor.beginEditBehavior(editor, behavior)
    }

    function handleRunOnClick() {
        const blueprint = toBlueprint(behaviors, connections)
        if (!blueprint) {
            showNotification({
                title: 'Error',
                body: 'Invalid behavior tree configuration',
                variant: 'warning'
            })
            return
        }

        try {
            const handle = pineBT.create(JSON.stringify(blueprint))
            pineBT.run(handle)
            const statuses = pineBT.status(handle)
            Document.updateBehaviorStatuses(document, statuses)
            pineBT.destroy(handle)
        } catch (error) {
            console.error(error)
            showNotification({
                title: 'Error',
                body: error.message,
                variant: 'danger',
            })
        }
    }

    function showNotification(notification) {
        const notificationWithId = {
            ...notification,
            id: Date.now()
        }
        setNotifications([...notifications, notificationWithId])
    }

    function hideNotification(id) {
        setNotifications(notifications.filter(n => n.id !== id))
    }

    function handleNewDocument() {
        Document.reset(document)
        Editor.reset(editor)
    }

    function showUnsavedChangesModal(next) {
        setModal({
            title: 'Unsaved changes',
            body: 'The current behavior tree has unsaved changes.',
            buttons: [
                {
                    title: 'Discard',
                    variant: 'danger'
                },
                {
                    title: 'Save',
                    variant: 'primary',
                    onClick: () => handleSaveDocument(),
                },
            ],
            next,
        })
    }

    useEffect(() => {
        return window.menu.file.new(() => {
            if (isDirty) {
                showUnsavedChangesModal(() => handleNewDocument())
            } else {
                handleNewDocument()
            }
        })
    }, [isDirty, documentData, documentFilePath])

    function handleOpenDocument(file) {
        try {
            Document.open(document, file)
            Editor.reset(editor)
        } catch (error) {
            showNotification({
                title: 'Failed to open document',
                body: error.message,
                variant: 'danger',
            })
        }
    }

    useEffect(() => {
        return window.menu.file.open((_, file) => {
            if (isDirty) {
                showUnsavedChangesModal(() => handleOpenDocument(file))
            } else {
                handleOpenDocument(file)
            }
        })
    }, [isDirty, documentData, documentFilePath])

    async function handleSaveDocument(path) {
        try {
            await Document.save(document, path)
        } catch (error) {
            showNotification({
                title: 'Failed to save file',
                body: error.message,
                variant: 'danger'
            })
        }
    }

    useEffect(() => {
        return window.menu.file.save((_, path) => {
            if (path) {
                handleSaveDocument(path)
            } else {
                handleSaveDocument()
            }
        })
    }, [documentFilePath, documentData])

    useEffect(() => {
        if (documentFilePath) {
            window.electron.setTitle(`pineBT Editor - ${documentFilePath}${isDirty ? '*' : ''}`)
        } else {
            window.electron.setTitle(`pineBT Editor - New file`)
        }
    }, [documentFilePath, isDirty])

    return (<>
        <Canvas
            editor={editor}
        >
        {
            behaviors.map(behavior =>
                <BehaviorCard 
                    key={behavior.id}
                    behavior={behavior}
                    document={document}
                    editor={editor}
                    onEdit={() => handleBehaviorCardEdit(behavior)}
                />
            )
        }
            <svg className={bem('canvas', 'connections')}>
            {
                connections.map(connection => 
                    <ConnectionLine
                        key={`${connection.from}->${connection.to}`}
                        from={behaviors.find(b => b.id == connection.from)}
                        to={behaviors.find(b => b.id == connection.to)}
                        onClick={() => handleConnectionOnClick(connection)}
                    />
                )
            }
            {
                newConnection
                    ? <ConnectionLine
                        from={behaviors.find(b => b.id == newConnection.from)}
                        to={{ position: { x: mousePosition.x - 1, y: mousePosition.y - 3 } }}
                        />
                    : null
            }
            </svg>
        </Canvas>
        <BehaviorDrawer
            schemas={schemas}
            show={showBehaviorDrawer}
            onHide={hideBehaviorDrawer}
            onSelectSchema={handleBehaviorDrawerOnSelectSchema}
        />
        <BehaviorEdit
            behavior={Editor.inEditBehavior(editor)}
            show={showBehaviorEdit}
            document={document}
            onHide={() => Editor.endEditBehavior(editor)}
        />
        <BlackboardDrawer
            document={document}
            show={showBlackboardDrawer}
            onHide={() => setShowBlackboardDrawer(false)}
        />
        <Button 
            id='run-button'
            onClick={handleRunOnClick}
        >
            <Stack direction='horizontal' gap={2}>
                <i className='bi bi-play-fill'/>
                <span>Run</span>
            </Stack>
        </Button>
        <Button
            id='blackboard-button'
            variant='secondary'
            onClick={() => setShowBlackboardDrawer(true)}
        >
            <Stack direction='horizontal' gap={2}>
                <i className='bi bi-clipboard-fill'/>
                <span>Blackboard</span>
            </Stack>
        </Button>
        <ToastContainer
            id='notification-container'
            className='p-3'
            position='top-end'
        >
        {
            notifications.map(notification => 
                <Toast
                    key={notification.id}
                    bg={notification.variant}
                    show={true}
                    autohide={true}
                    delay={7000}
                    onClose={() => hideNotification(notification.id)}
                >
                    <Toast.Header>
                        <strong className='me-auto'>{notification.title}</strong>
                    </Toast.Header>
                    <Toast.Body>{notification.body}</Toast.Body>
                </Toast>
            )
        }
        </ToastContainer>
        <Modal 
            show={!!modal} 
            onHide={() => setModal(null)}
            backdrop='static'
        >
            <Modal.Header closeButton>
                <Modal.Title>{modal && modal.title}</Modal.Title>
            </Modal.Header>
            <Modal.Body>{modal && modal.body}</Modal.Body>
            <Modal.Footer>
            {
                modal && modal.buttons.map((button, index) => 
                    <Button
                        key={index}
                        variant={button.variant || 'secondary'}
                        onClick={async () => {
                            button.onClick && (await button.onClick())
                            setModal(null)
                            modal.next && modal.next()
                        }}
                    >
                        {button.title}
                    </Button>
                )
            }
            </Modal.Footer>
        </Modal>
    </>)
}
