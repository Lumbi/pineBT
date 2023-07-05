import { useState, useRef, useEffect } from 'react'
import { Button } from 'react-bootstrap'
import bem from '../bem'
import * as Behavior from '../models/behavior'
import * as Document from '../models/document'
import * as Editor from '../models/editor'

import './behavior-card.less'

export default function BehaviorCard(props) {
    const { 
        behavior,
        schema,
        document,
        editor,
        onEdit,
    } = props

    const {
        behaviors,
        connections,
    } = document

    const self = useRef()
    const isRoot = Behavior.isRoot(behavior)
    const position = behavior.position;
    const [isDragging, setDragging] = useState(false)
    const [lastMousePosition, setLastMousePosition] = useState({ x: 0, y: 0 })

    const hasParent = !!connections.find(c => c.to === behavior.id)
    const hasChildren = !!connections.find(c => c.from === behavior.id)
    const canEdit = !isRoot
    const allowsChildren = Document.allowsChildren(document, behavior)
    const canAddChild = Document.canAddChildToBehavior(document, behavior)

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

            function isDescendantOf(ancestor, descendant) {
                if (connections.find(c => c.from === ancestor && c.to === descendant)) {
                    return true
                }

                const parentConnection = connections.find(c => c.to === descendant)
                if (parentConnection) {
                    return isDescendantOf(ancestor, parentConnection.from)
                }

                return false
            }

            Document.updateBehaviors(
                document,
                behaviors.map(b => {
                    const flag = b.id === behavior.id || isDescendantOf(behavior.id, b.id)
                    if (flag) {
                        return {
                            ...b,
                            position: { x: b.position.x + delta.x, y: b.position.y + delta.y }
                        }
                    } else {
                        return b
                    }
                })
            )

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

    function handleParentHandleClick(event) {
        const { newConnection } = editor
        if (newConnection) {
            event.preventDefault()
            event.stopPropagation()
            if (newConnection) {
                Document.addConnection(document, { ...newConnection, to: behavior.id })
            }
            Editor.endNewConnection()
        }
    }

    function handleChildrenHandleClick(event) {
        const { newConnection } = editor
        if (!newConnection && canAddChild) {
            event.preventDefault()
            event.stopPropagation()
            Editor.beginNewConnection(editor, behavior.id)
        }
    }

    function statusIcon() {
        switch (behavior.status) {
            case 'running': return 'bi-stopwatch-fill'
            case 'failure': return 'bi-dash-circle-fill'
            case 'success': return 'bi-check-circle-fill'
            case 'aborted': return 'bi-slash-circle-fill'
            default: undefined
        }
    }

    return (
        <div 
            ref={self}
            id={`behavior-card-${behavior.id}`}
            className={bem('behavior-card', null, { 
                grabbing: isDragging
            })}
            style={{
                zIndex: position.y
            }}
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
                    className={bem('behavior-card', 'parent-handle', { 
                        connected: hasParent,
                        hidden: isRoot,
                    })}
                    onClick={handleParentHandleClick}
                    onMouseDown={(event) => event.stopPropagation()}
                    onMouseMove={(event) => event.stopPropagation()}
                />
                <p>{behavior.schema}</p>
                <div 
                    className={bem('behavior-card', 'children-handle', { 
                        connected: hasChildren,
                        hidden: !allowsChildren,
                    })}
                    onClick={handleChildrenHandleClick}
                    onMouseDown={(event) => event.stopPropagation()}
                    onMouseMove={(event) => event.stopPropagation()}
                />
            </div>
            <div className={bem('behavior-card', 'status', { 
                hidden: !statusIcon(),
                [behavior.status]: true
            })}>
            {
                <i className={`${statusIcon()}`}></i>
            }
            </div>
            <div className={bem('behavior-card', 'edit', { hidden: !canEdit })}>
                <Button 
                    variant='outline-secondary' 
                    size='sm'
                    onClick={onEdit}
                >
                    <i className='bi bi-gear-fill'></i>
                </Button>
            </div>
        </div>
    )
}
