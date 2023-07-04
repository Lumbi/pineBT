import { useState, useEffect, useMemo } from 'react'
import { Button, Toast, ToastContainer } from 'react-bootstrap'
import BehaviorCard from './behavior-card'
import BehaviorConnection from './behavior-connection'
import BehaviorDrawer from './behavior-drawer'
import { BehaviorEdit } from './behavior-edit'
import { loadBehaviorSchemas } from '../behavior-schema'
import bem from '../bem'

import 'bootstrap/dist/css/bootstrap.min.css'
import './app.less'

const rootBehavior = {
    schema: 'Root',
    id: 0,
    position: { x: 400, y: 100 }
}

function toBlueprint(behaviors, connections) {
    const entryConnection = connections.find(c => c.from === 0)
    if (!entryConnection) return undefined
    const entryBehavior= behaviors.find(b => b.id === entryConnection.to)
    if (!entryBehavior) return undefined

    function recursively(behavior) {
        const childConnections = connections.filter(c => c.from === behavior.id)
        const childBehaviors = childConnections
            .map(c => behaviors.find(b => b.id === c.to))
            .filter(b => !!b)
            .sort((a, b) => a.position.x < b.position.x)
            .map(recursively)

        return {
            id: behavior.id,
            schema: behavior.schema,
            options: behavior.options,
            children: childBehaviors
        }
    }

    return {
        root: recursively(entryBehavior)
    }
}

export default function App() {
    const [schemas, setSchemas] = useState([])
    const [behaviors, setBehaviors] = useState([rootBehavior])
    const [connections, setConnections] = useState([])
    const [newConnection, setNewConnection] = useState()
    const [mousePosition, setMousePosition] = useState()
    const [scrollOffset, setScrollOffset] = useState({ x: 0, y: 0 })
    const [isDragging, setDragging] = useState(false)
    const [showBehaviorDrawer, setShowBehaviorDrawer] = useState(false)
    const [showBehaviorEdit, setShowBehaviorEdit] = useState(false)
    const [inEditBehaviorId, setInEditBehaviorId] = useState()
    const [notifications, setNotifications] = useState([])

    const [documentFilePath, setDocumentFilePath] = useState()

    const documentData = useMemo(() => {
        return JSON.stringify({
            behaviors,
            connections,
        }, undefined, 4)
    }, [behaviors, connections])

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

    useEffect(() => {
        window.scrollTo({
            left: -scrollOffset.x, 
            top: -scrollOffset.y,
            behavior: 'instant'
        })
    }, [scrollOffset])

    function handleCanvasOnMouseDown(event) {
        event.preventDefault()
        setDragging(true)

        if (newConnection) {
            setShowBehaviorDrawer(true)
            setDragging(false)
        }
    }

    function handleCanvasOnMouseMove(event) {
        setMousePosition({ x: event.pageX, y: event.pageY })

        if (isDragging) {
            event.preventDefault()
            setScrollOffset({
                x: scrollOffset.x + event.movementX,
                y: scrollOffset.y + event.movementY
            })
        }
    }

    function handleCanvasOnMouseUp(event) {
        if (isDragging) {
            event.preventDefault()
            setDragging(false)
        }
    }

    function handleEditBehavior(behavior) {
        updateBehavior(behavior)
    }

    function handleDeleteBehavior(behavior) {
        setShowBehaviorEdit(false)
        setTimeout(() => {
            deleteBehaviorById(behavior.id)
        }, 300)
    }

    function updateBehavior(behaviorUpdate) {
        setBehaviors(
            behaviors.map(behavior => {
                if (behavior.id === behaviorUpdate.id) {
                    return {...behavior, ...behaviorUpdate}
                } else {
                    return behavior
                }
            })
        )
    }

    function updateBehaviors(behaviors) {
        setBehaviors(behaviors)
    }

    function deleteBehaviorById(behaviorId) {
        setConnections(
            connections.filter(c => c.from !== behaviorId && c.to !== behaviorId)
        )

        setBehaviors(
            behaviors.filter(b => b.id !== behaviorId)
        )
    }

    function beginNewConnection(from) {
        setNewConnection({ from })
    }

    function schemaForBehavior(behavior) {
        if (!behavior) { return undefined }
        return schemas.find(s => s.name === behavior.schema)
    }

    function childrenOfBehavior(behaviorId) {
        const behavior = behaviors.filter(b => b.id === behaviorId)[0]
        if (!behavior) { return [] }

        const childConnections = connections.filter(c => c.from === behaviorId)
        const childBehaviors = childConnections
            .map(c => c.to)
            .flatMap(to => behaviors.filter(b => b.id === to))

        return childBehaviors
    }

    function canBehaviorAddChild(behaviorId) {
        const behavior = behaviors.filter(b => b.id === behaviorId)[0]
        if (!behavior) { return false }

        if (behavior.id === 0) { // root
            return childrenOfBehavior(behaviorId).length === 0
        }

        const behaviorSchema = schemas.find(s => s.name === behavior.schema)
        if (!behaviorSchema) { return false }

        if (behaviorSchema.hierarchy === "none") {
            return false
        } else if (behaviorSchema.hierarchy === "one") {
            return childrenOfBehavior(behaviorId).length === 0
        } else if (behaviorSchema.hierarchy === "many") {
            return true
        }

        return false
    }

    function commitNewConnection(to) {
        if (newConnection) {
            const connection = { ...newConnection, to }
            const alreadyExists = !!connections.find(c => c.from === connection.from && c.to === connection.to)
            const connectionToSelf = connection.from === connection.to
            const alreadyHasParent = !!connections.find(c => c.to === connection.to)
            const isValid = !alreadyExists && 
                            !connectionToSelf && 
                            !alreadyHasParent &&
                            canBehaviorAddChild(connection.from)
            if (isValid) {
                setConnections([...connections, connection])
            }
        }
        setNewConnection(undefined)
    }

    function cancelNewConnection() {
        setNewConnection(undefined)
    }

    function deleteConnection(from, to) {
        setConnections(
            connections.filter(c => c.from !== from || c.to !== to)
        )
    }

    function hideBehaviorDrawer() {
        cancelNewConnection()
        setShowBehaviorDrawer(false)
    }

    function createBehaviorFromSchema(schema) {
        const maxBehaviorId = behaviors.map(b => b.id).sort().slice(-1)[0]
        const nextBehaviorId = maxBehaviorId + 1

        function defaultValueForOptionType(type) {
            if (type === 'boolean') {
                return false
            } else if (type === 'number') {
                return 0
            } else if (typeof type === 'number') { // enumeration
                return { case: 0 }
            }
            return undefined
        }

        const newBehavior = {
            schema: schema.name,
            id: nextBehaviorId,
            position: mousePosition,
            options: schema.options && Object.fromEntries(
                Object.entries(schema.options).map(([key, type]) => [
                    key,
                    defaultValueForOptionType(type)
                ])
            )
        }

        setBehaviors([...behaviors, newBehavior])

        if (newConnection) {
            commitNewConnection(newBehavior.id)
        }
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

    useEffect(() => {
        return window.menu.on.file.save(() => {
            window.electron
                .saveFile({
                    path: documentFilePath,
                    data: documentData
                })
                .then(result => {
                    if (result.path) {
                        setDocumentFilePath(result.path)
                        setSavedDocumentData(documentData)
                    }
                })
                .catch((error) => {
                    showNotification({
                        title: 'Failed to save file',
                        body: error.message,
                        variant: 'danger'
                    })
                })
        })
    }, [documentFilePath, documentData])

    useEffect(() => {
        return window.menu.on.file.open((_, document) => {
            try {
                const { path, data } = document
                const { behaviors, connections } = JSON.parse(data)
                setBehaviors(behaviors)
                setConnections(connections)
                setDocumentFilePath(path)
                setSavedDocumentData(undefined)
            } catch (error) {
                showNotification({
                    title: 'Failed to open document',
                    body: error.message,
                    variant: 'danger',
                })
            }
        })
    }, [])

    useEffect(() => {
        if (documentFilePath) {
            window.electron.setTitle(`pineBT Editor - ${documentFilePath}${isDirty ? '*' : ''}`)
        } else {
            window.electron.setTitle(`pineBT Editor - New file`)
        }
    }, [documentFilePath, isDirty])

    return (<>
        <div 
            className={bem('canvas', null, { connecting: !!newConnection })}
            onMouseDown={handleCanvasOnMouseDown}
            onMouseMove={handleCanvasOnMouseMove}
            onMouseUp={handleCanvasOnMouseUp}
        >
            <div 
                className={bem('canvas', 'viewport')}
            >
                {
                    behaviors.map(behavior =>
                        <BehaviorCard 
                            key={behavior.id}
                            behavior={behavior}
                            schema={schemas.find(s => s.name === behavior.schema)}
                            updateBehaviors={updateBehaviors}
                            behaviors={behaviors}
                            connections={connections}
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
                            onClick={() => deleteConnection(connection.from, connection.to)}
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
            </div>
        </div>
        <BehaviorDrawer
            schemas={schemas}
            show={showBehaviorDrawer}
            onHide={hideBehaviorDrawer}
            onSelectSchema={createBehaviorFromSchema}
        />
        <BehaviorEdit
            behavior={inEditBehavior}
            schema={schemaForBehavior(inEditBehavior)}
            show={showBehaviorEdit}
            onHide={() => setShowBehaviorEdit(false)}
            onDelete={handleDeleteBehavior}
            onEdit={handleEditBehavior}
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
    </>)
}
