import { useState, useEffect } from 'react'
import BehaviorCard from './behavior-card'
import BehaviorConnection from './behavior-connection'
import BehaviorDrawer from './behavior-drawer'
import { BehaviorEdit } from './behavior-edit'
import { loadBehaviorSchemas } from '../behavior-schema'
import bem from '../bem'

import 'bootstrap/dist/css/bootstrap.min.css'
import './app.less'

const testBehaviors = [
    {
        schema: 'Root',
        id: 0,
        position: { x: 50, y: 50 }
    },
    {
        schema: 'Parallel',
        id: 2,
        position: { x: 50, y: 200 }
    },
    {
        schema: 'Selector',
        id: 100,
        position: { x: 50, y: 400 }
    },
    {
        schema: 'MockTask',
        id: 3,
        position: { x: 400, y: 200 }
    },
    {
        schema: 'MockTask',
        id: 4,
        position: { x: 400, y: 400 }
    }
]

const testConnections = [
    { from: 0, to: 2 }
]

export default function App() {
    const [schemas, setSchemas] = useState([])
    const [behaviors, setBehaviors] = useState(testBehaviors)
    const [connections, setConnections] = useState(testConnections)
    const [newConnection, setNewConnection] = useState()
    const [mousePosition, setMousePosition] = useState()
    const [scrollOffset, setScrollOffset] = useState({ x: 0, y: 0 })
    const [isDragging, setDragging] = useState(false)
    const [showBehaviorDrawer, setShowBehaviorDrawer] = useState(false)
    const [showBehaviorEdit, setShowBehaviorEdit] = useState(false)
    const [inEditBehavior, setInEditBehavior] = useState()

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
                    return {... behavior, ...behaviorUpdate}
                } else {
                    return behavior
                }
            })
        )
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

        const newBehavior = {
            schema: schema.name,
            id: nextBehaviorId,
            position: mousePosition
        }

        setBehaviors([...behaviors, newBehavior])

        if (newConnection) {
            commitNewConnection(newBehavior.id)
        }
    }

    function showEditForBehavior(behavior) {
        setInEditBehavior(behavior)
        setShowBehaviorEdit(true)
    }

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
                            updateBehavior={updateBehavior}
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
        />
    </>)
}
