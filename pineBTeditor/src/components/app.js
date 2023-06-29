import { useState, useRef, useEffect } from 'react'
import BehaviorCard from './behavior-card'
import BehaviorConnection from './behavior-connection'
import bem from '../bem'
import BehaviorDrawer from './behavior-drawer'

import 'bootstrap/dist/css/bootstrap.min.css'
import './app.less'

const testBehaviors = [
    {
        schema: 'Root',
        id: 1,
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
        schema: 'Task',
        id: 3,
        position: { x: 400, y: 200 }
    },
    {
        schema: 'Task',
        id: 4,
        position: { x: 400, y: 400 }
    }
]

const testConnections = [
    { from: 1, to: 2 }
]

export default function App() {
    const [behaviors, setBehaviors] = useState(testBehaviors)
    const [connections, setConnections] = useState(testConnections)
    const [newConnection, setNewConnection] = useState()
    const [mousePosition, setMousePosition] = useState()
    const [scrollOffset, setScrollOffset] = useState({ x: 0, y: 0 })
    const [isDragging, setDragging] = useState(false)
    const [showBehaviorDrawer, setShowBehaviorDrawer] = useState(false)

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
            cancelNewConnection()
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

    function beginNewConnection(from) {
        setNewConnection({ from })
    }

    function commitNewConnection(to) {
        if (newConnection) {
            const connection = { ...newConnection, to }
            const alreadyExists = !!connections.find(c => c.from === connection.from && c.to === connection.to)
            const connectionToSelf = connection.from === connection.to
            const alreadyHasParent = !!connections.find(c => c.to === connection.to)
            const isValid = !alreadyExists && !connectionToSelf && !alreadyHasParent
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
                            updateBehavior={updateBehavior}
                            connections={connections}
                            newConnection={newConnection}
                            beginNewConnection={beginNewConnection}
                            commitNewConnection={commitNewConnection}
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
            show={showBehaviorDrawer}
            onHide={() => setShowBehaviorDrawer(false)}
        />
    </>)
}
