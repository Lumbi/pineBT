import { useState, useRef, useEffect } from 'react'
import bem from '../bem'

import './behavior-card.less'

export default function BehaviorCard(props) {
    const { 
        behavior,
        updateBehavior,
        connections
    } = props

    const self = useRef()
    const position = behavior.position;
    const [isDragging, setDragging] = useState(false)
    const [lastMousePosition, setLastMousePosition] = useState({ x: 0, y: 0 })

    const hasParent = !!connections.find(c => c.to === behavior.id)
    const hasChildren = !!connections.find(c => c.from === behavior.id)

    useEffect(() => {
      if (self.current) {
        self.current.style.transform = `translate(${position.x}px, ${position.y}px)`
      }
    }, [position])

    function updateLastMousePosition(event) {
        setLastMousePosition({ x: event.screenX, y: event.screenY })
    }

    function handleMouseDown(event) {
        event.preventDefault()
        event.stopPropagation()
        setDragging(true)
        updateLastMousePosition(event)
    }

    function handleMouseMove(event) {
        if (isDragging) {
            event.preventDefault()
            event.stopPropagation()
            const delta = {
                x: event.screenX - lastMousePosition.x,
                y: event.screenY - lastMousePosition.y 
            }
            updateBehavior({
                id: behavior.id,
                position: { x: position.x + delta.x, y: position.y + delta.y }
            })
            updateLastMousePosition(event)
        }
    }

    function handleMouseUp(event) {
        if (isDragging) {
            event.preventDefault()
            event.stopPropagation()
            setDragging(false)
        }
    }

    function handleMouseLeave(event) {
        if (isDragging) {
            event.preventDefault()
            setDragging(false)
        }
    }

    return (
        <div 
            ref={self}
            id={`behavior-card-${behavior.id}`}
            className={bem('behavior-card', null, { grabbing: isDragging })}
        >
            <div 
                className={bem('behavior-card', 'background')}
                onMouseDown={handleMouseDown}
                onMouseMove={handleMouseMove}
                onMouseUp={handleMouseUp}
                onMouseLeave={handleMouseLeave}
            />
            <div className={bem('behavior-card', 'content')}>
                <div 
                    className={bem('behavior-card', 'parent-handle', { connected: hasParent })}
                    onClick={(event) => {
                        event.preventDefault()
                        event.stopPropagation()
                    }}
                    onMouseDown={(event) => event.stopPropagation()}
                    onMouseMove={(event) => event.stopPropagation()}
                />
                <p>{behavior.schema}</p>
                <div 
                    className={bem('behavior-card', 'children-handle', { connected: hasChildren })}
                    onMouseDown={(event) => event.stopPropagation()}
                    onMouseMove={(event) => event.stopPropagation()}
                />
            </div>
        </div>
    )
}
