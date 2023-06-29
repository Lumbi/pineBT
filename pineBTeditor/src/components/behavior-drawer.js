import { useEffect, useState } from 'react'
import Offcanvas from 'react-bootstrap/Offcanvas'
import Button from 'react-bootstrap/Button'
import Stack from 'react-bootstrap/Stack'
import { loadBehaviorSchemas } from '../behavior-schema'

import './behavior-drawer.less'

export default function BehaviorDrawer(props) {
    const {
        show,
        onHide,
        onSelectSchema
    } = props

    const [schemas, setSchemas] = useState([])

    useEffect(() => {
        async function loadSchemas() {
            setSchemas(await loadBehaviorSchemas())
        }

        loadSchemas()
            .catch(console.error)
    }, [])

    function BehaviorButton(props) {
        const { schema, onClick } = props
        return (
            <Button 
                variant="secondary" 
                size="lg"
                onClick={onClick}
            >
                {schema.name}
            </Button>
        )
    }

    return (
        <Offcanvas id="behavior-drawer" show={show} onHide={onHide}>
            <Offcanvas.Header closeButton>
                <Offcanvas.Title>Behaviors</Offcanvas.Title>
            </Offcanvas.Header>
            <Offcanvas.Body>
                <Stack gap={3}>
                {
                    schemas.map(schema => 
                        <BehaviorButton
                            key={schema.name}
                            schema={schema}
                            onClick={() => {
                                onSelectSchema(schema)
                                onHide()
                            }}
                        />
                    )
                }
                </Stack>
            </Offcanvas.Body>
        </Offcanvas>
    )
}