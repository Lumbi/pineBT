import { useState, useRef, useEffect } from 'react'

import './behavior-card.less'

export default function BehaviorCard(props) {
    const { 
        behavior,
        updateBehavior,
    } = props

    const position = behavior.position;
    const [isDragging, setDragging] = useState(false)

    const self = useRef()

    useEffect(() => {
      if (self.current) {
        self.current.style.transform = `translate(${position.x}px, ${position.y}px)`
      }
    }, [position])

    function handleMouseDown(event) {
        event.preventDefault()
        event.stopPropagation()
        setDragging(true)
    }

    function handleMouseMove(event) {
        if (isDragging) {
            event.preventDefault()
            event.stopPropagation()
            updateBehavior({
                id: behavior.id,
                position: {
                    x: position.x + event.movementX,
                    y: position.y + event.movementY
                }
            })
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
            className={`behavior-card ${isDragging ? 'behavior-card--grabbing' : ''}`}
        >
            <div 
                className='behavior-card__background'
                onMouseDown={handleMouseDown}
                onMouseMove={handleMouseMove}
                onMouseUp={handleMouseUp}
                onMouseLeave={handleMouseLeave}
            />
            <div className='behavior-card__content'>
                <p>{behavior.schema}</p>
                <div className='behavior-card__handle'></div>
            </div>
        </div>
    )
}
