import { useState, useRef, useEffect } from 'react'
import BehaviorCard from './behavior-card'

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
    }
]

export default function App() {
    const [behaviors, setBehaviors] = useState(testBehaviors)
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
        event.preventDefault()
        setDragging(false)
    }

    return <>
        <div 
            className='behavior-canvas'
            onMouseDown={handleCanvasOnMouseDown}
            onMouseMove={handleCanvasOnMouseMove}
            onMouseUp={handleCanvasOnMouseUp}
        >
            <div 
                className='behavior-viewport'
                ref={behaviorViewport}
            >
            {
                behaviors.map(behavior =>
                    <BehaviorCard behavior={behavior}/>
                )
            }
            </div>
        </div>
    </>
}