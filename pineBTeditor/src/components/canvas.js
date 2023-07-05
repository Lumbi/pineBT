import { useEffect } from 'react'
import bem from '../bem'
import * as Editor from '../models/editor'

import './canvas.less'

export default function Canvas(props) {
    const { editor } = props
    const { children } = props

    function handleCanvasOnMouseDown(event) {
        event.preventDefault()
        editor.setDragging(true)

        if (editor.newConnection) {
            editor.setShowBehaviorDrawer(true)
            editor.setDragging(false)
        }
    }

    function handleCanvasOnMouseMove(event) {
        editor.setMousePosition({ x: event.pageX, y: event.pageY })

        if (editor.isDragging) {
            event.preventDefault()
            editor.setScrollOffset({
                x: editor.scrollOffset.x + event.movementX,
                y: editor.scrollOffset.y + event.movementY
            })
        }
    }

    function handleCanvasOnMouseUp(event) {
        if (editor.isDragging) {
            event.preventDefault()
            editor.setDragging(false)
        }
    }

    useEffect(() => {
        window.scrollTo({
            left: -editor.scrollOffset.x, 
            top: -editor.scrollOffset.y,
            behavior: 'instant'
        })
    }, [editor.scrollOffset])

    return (
        <div 
            className={bem('canvas', null, { connecting: !!editor.newConnection })}
            onMouseDown={handleCanvasOnMouseDown}
            onMouseMove={handleCanvasOnMouseMove}
            onMouseUp={handleCanvasOnMouseUp}
        >
            <div 
                className={bem('canvas', 'viewport')}
            >
            {
                children
            }
            </div>
        </div>
    )
}
