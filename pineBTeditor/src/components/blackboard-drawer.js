import { Stack, Offcanvas, Form, Button } from 'react-bootstrap'
import * as Document from '../models/document'
import * as Execution from '../models/execution'
import bem from '../bem'

import './blackboard-drawer.less'
import { useRef } from 'react'

export default function BlackboardDrawer(props) {
    const {
        show,
        document,
        execution,
        onHide,
    } = props

    const {
        blackboard
    } = document

    const bottomRef = useRef()

    function handleAddOnClick() {
        Document.addBlackboardEntry(document)
        bottomRef.current.scrollIntoView({ behavior: 'smooth' })
    }

    function handleEntryNameOnChange(entry, event) {
        Document.updateBlackboardEntry(document, { ...entry, name: event.target.value })
    }

    function handleEntryValueOnChange(entry, event) {
        Document.updateBlackboardEntry(document, { ...entry, value: event.target.valueAsNumber })
    }

    function handleEntryDeleteOnClick(entry) {
        Document.deleteBlackboardEntry(document, entry.key)
        Execution.clearBlackboardKey(execution, entry.key)
    }

    return (
        <Offcanvas 
            id='blackboard-drawer'
            show={show} 
            onHide={onHide} 
            placement='bottom'
            style={{ minHeight: 500 }}
        >
            <Offcanvas.Header closeButton>
                <Offcanvas.Title>Blackboard</Offcanvas.Title>
            </Offcanvas.Header>
            <Offcanvas.Body className={bem('blackboard-drawer', 'body')}>
                <Stack className={bem('blackboard-drawer', 'entry-list')} direction='vertical' gap={2}>
                {
                    blackboard.entries.map(entry => {
                        const { key, name, value } = entry
                        const id = `Blackboard_${key}`
                        return (
                            <Stack 
                                key={entry.key}
                                className={bem('blackboard-drawer', 'entry-row')}
                                direction='horizontal'
                                gap={2}
                            >
                                <Form.Label htmlFor={id}>{key}</Form.Label>
                                <Form.Control
                                    id={id}
                                    type='text'
                                    value={name}
                                    onChange={(event) => handleEntryNameOnChange(entry, event)}
                                />
                                <Form.Control
                                    type='number'
                                    value={isNaN(value) ? '' : value}
                                    onChange={(event) => handleEntryValueOnChange(entry, event)}
                                />
                                <Button
                                    variant='outline-danger'
                                    onClick={() => handleEntryDeleteOnClick(entry)}
                                >
                                    <i className='bi bi-trash3'/>
                                </Button>
                            </Stack>
                        )
                    })
                }
                </Stack>
                <div ref={bottomRef} />
                <Button 
                    className={bem('blackboard-drawer', 'add-entry-button')}
                    onClick={handleAddOnClick}
                >
                    <Stack direction='horizontal' gap={2}>
                        <i className='bi bi-plus-square'/>
                        <span>Add</span>
                    </Stack>
                </Button>
            </Offcanvas.Body>
        </Offcanvas>
    )
}