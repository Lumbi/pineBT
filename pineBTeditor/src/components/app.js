import { useState, useRef, useEffect } from 'react'
import BehaviorCard from './behavior-card'
import BehaviorConnection from './behavior-connection'
import bem from '../bem'

import './app.less'

const testBehaviors = [
    {
        schema: 'Root',
        id: 1,
        position: { x: 0, y: 0 }
    },
    {
        schema: 'Parallel',
        id: 2,
        position: { x: 0, y: 200 }
    },
    {
        schema: 'Task',
        id: 3,
        position: { x: 400, y: 200 }
    }
]

const testConnections = [
    { from: 1, to: 2 }
]

export default function App() {
    const [behaviors, setBehaviors] = useState(testBehaviors)
    const [connections, setConnections] = useState(testConnections)
    const [scrollOffset, setScrollOffset] = useState({ x: 0, y: 0 })

    const [isDragging, setDragging] = useState(false)

    const behaviorViewport = useRef()

    useEffect(() => {
      if (behaviorViewport.current) {
        behaviorViewport.current.style.transform = `translate(${scrollOffset.x}px, ${scrollOffset.y}px)`
      }
    }, [scrollOffset])

    function handleCanvasOnMouseDown(event) {
        event.preventDefault()
        setDragging(true)
    }

    function handleCanvasOnMouseMove(event) {
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

    return <>
        <div 
            className='canvas'
            onMouseDown={handleCanvasOnMouseDown}
            onMouseMove={handleCanvasOnMouseMove}
            onMouseUp={handleCanvasOnMouseUp}
        >
            <div 
                className={bem('canvas', 'viewport')}
                ref={behaviorViewport}
            >
                {
                    behaviors.map(behavior =>
                        <BehaviorCard 
                            key={behavior.id}
                            behavior={behavior}
                            updateBehavior={updateBehavior}
                            connections={connections}
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
                            />
                        )
                    }
                </svg>
            </div>
        </div>
    </>
}
