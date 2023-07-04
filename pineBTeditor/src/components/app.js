import { useState, useEffect, useMemo } from 'react'
import { Button, Toast, ToastContainer, Modal } from 'react-bootstrap'
import Canvas from './canvas'
import BehaviorCard from './behavior-card'
import BehaviorConnection from './behavior-connection'
import BehaviorDrawer from './behavior-drawer'
import { BehaviorEdit } from './behavior-edit'
import { loadBehaviorSchemas } from '../behavior-schema'
import { rootBehavior } from '../models/behavior'
import { toBlueprint } from '../models/blueprint'
import { addBehavior, newBehaviorFromSchema, addConnection, deleteConnection, resetDocument, toDocumentData } from '../models/document'
import bem from '../bem'

import 'bootstrap/dist/css/bootstrap.min.css'
import './app.less'

export default function App() {
    const [schemas, setSchemas] = useState([])
    const [behaviors, setBehaviors] = useState([rootBehavior()])
    const [connections, setConnections] = useState([])
    const [newConnection, setNewConnection] = useState()
    const [mousePosition, setMousePosition] = useState()
    const [scrollOffset, setScrollOffset] = useState({ x: 0, y: 0 })
    const [isDragging, setDragging] = useState(false)
    const [showBehaviorDrawer, setShowBehaviorDrawer] = useState(false)
    const [showBehaviorEdit, setShowBehaviorEdit] = useState(false)
    const [inEditBehaviorId, setInEditBehaviorId] = useState()
    const [notifications, setNotifications] = useState([])
    const [modal, setModal] = useState(null)

    const [documentFilePath, setDocumentFilePath] = useState()

    const document = {
        behaviors,
        setBehaviors,
        connections,
        setConnections,
        schemas,
    }

    const editor = {
        mousePosition,
        setMousePosition,
        scrollOffset,
        setScrollOffset,
        isDragging,
        setDragging,
        newConnection,
        setNewConnection,
        setShowBehaviorDrawer,
    }

    const documentData = useMemo(
        () => toDocumentData(document), 
        [behaviors, connections]
    )

    const [savedDocumentData, setSavedDocumentData] = useState()
    if (!savedDocumentData) {
        setSavedDocumentData(documentData)
    }
    const isDirty = savedDocumentData != documentData

    useEffect(() => {
        async function loadSchemas() {
            setSchemas(await loadBehaviorSchemas())
        }

        loadSchemas()
            .catch(console.error)
    }, [])

    function beginNewConnection(from) {
        setNewConnection({ from })
    }

    function commitNewConnection(to) {
        if (newConnection) {
            addConnection(document, { ...newConnection, to })
        }
        setNewConnection(undefined)
    }

    function cancelNewConnection() {
        setNewConnection(undefined)
    }

    function handleBehaviorDrawerOnSelectSchema(schema) {
        const newBehavior = newBehaviorFromSchema(document, schema)
        const newBehaviorAtMousePosition = {
            ...newBehavior,
            position: mousePosition
        }
        addBehavior(document, newBehaviorAtMousePosition)
        if (newConnection) {
            commitNewConnection(newBehavior.id)
        }
    }

    function handleConnectionOnClick(connection) {
        deleteConnection(document, connection)
    }

    function hideBehaviorDrawer() {
        cancelNewConnection()
        setShowBehaviorDrawer(false)
    }

    function showEditForBehavior(behavior) {
        setInEditBehaviorId(behavior.id)
        setShowBehaviorEdit(true)
    }

    const inEditBehavior = behaviors.find(b => b.id === inEditBehaviorId)

    function updateBehaviorStatuses(statuses) {
        setBehaviors(
            behaviors.map(b => {
                const found = statuses.find(s => s.id === b.id)
                return { ...b, status: found && found.status && found.status.toLowerCase() }
            })
        )
    }

    function handleRunOnClick() {
        const blueprint = toBlueprint(behaviors, connections)
        if (blueprint) {
            const handle = pineBT.create(JSON.stringify(blueprint))
            pineBT.run(handle)
            const statuses = pineBT.status(handle)
            updateBehaviorStatuses(statuses)
            pineBT.destroy(handle)
        } else {
            showNotification({
                title: 'Error',
                body: 'Invalid behavior tree configuration',
                variant: 'warning'
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

    function newDocument() {
        resetDocument(document)
        setDocumentFilePath(undefined)
        setSavedDocumentData(undefined)
        setScrollOffset({ x: 0, y: 0 })
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
                    onClick: () => saveDocument(documentFilePath),
                },
            ],
            next,
        })
    }

    useEffect(() => {
        return window.menu.on.file.new(() => {
            if (isDirty) {
                showUnsavedChangesModal(() => newDocument())
            } else {
                newDocument()
            }
        })
    }, [isDirty, documentData, documentFilePath])

    function openDocument(document) {
        try {
            const { path, data } = document
            const { behaviors, connections } = JSON.parse(data)
            setBehaviors(behaviors)
            setConnections(connections)
            setDocumentFilePath(path)
            setSavedDocumentData(undefined)
            setScrollOffset({ x: 0, y: 0 })
        } catch (error) {
            showNotification({
                title: 'Failed to open document',
                body: error.message,
                variant: 'danger',
            })
        }
    }

    useEffect(() => {
        return window.menu.on.file.open((_, document) => {
            if (isDirty) {
                showUnsavedChangesModal(() => openDocument(document))
            } else {
                openDocument(document)
            }
        })
    }, [isDirty, documentData, documentFilePath])

    async function saveDocument(path) {
        try {
            const result = await window.electron.saveFile({
                path: path,
                data: documentData
            })
            if (result.path) {
                setDocumentFilePath(result.path)
                setSavedDocumentData(documentData)
            }
        } catch (error) {
            showNotification({
                title: 'Failed to save file',
                body: error.message,
                variant: 'danger'
            })
        }
    }

    useEffect(() => {
        return window.menu.on.file.save((_, path) => {
            if (path) {
                saveDocument(path)
            } else {
                saveDocument(documentFilePath)
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
                            schema={schemas.find(s => s.name === behavior.schema)}
                            document={document}
                            newConnection={newConnection}
                            beginNewConnection={beginNewConnection}
                            commitNewConnection={commitNewConnection}
                            onEdit={() => showEditForBehavior(behavior)}
                        />
                    )
                }
                <svg className={bem('canvas', 'connections')}>
                {
                    connections.map(connection => 
                        <BehaviorConnection
                            key={`${connection.from}->${connection.to}`}
                            from={behaviors.find(b => b.id == connection.from)}
                            to={behaviors.find(b => b.id == connection.to)}
                            onClick={() => handleConnectionOnClick(connection)}
                        />
                    )
                }
                {
                    newConnection
                        ? <BehaviorConnection
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
            behavior={inEditBehavior}
            show={showBehaviorEdit}
            document={document}
            onHide={() => setShowBehaviorEdit(false)}
        />
        <Button 
            id='run-button'
            onClick={handleRunOnClick}
        >
            Run
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
                    autohide={false}
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
