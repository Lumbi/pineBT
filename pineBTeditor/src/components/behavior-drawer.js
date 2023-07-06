import { useMemo } from 'react'
import Offcanvas from 'react-bootstrap/Offcanvas'
import Button from 'react-bootstrap/Button'
import Stack from 'react-bootstrap/Stack'
import { byPreferredOrder } from '../sort'

import './behavior-drawer.less'

export default function BehaviorDrawer(props) {
    const {
        schemas,
        show,
        onHide,
        onSelectSchema
    } = props

    const sortedSchemas = useMemo(
        () => [...schemas].sort(byPreferredOrder(
            [
                'Selector',
                'Sequence',
                'Parallel',
                'BlackboardCondition',
                'MockCondition',
                'MockTask'
            ], schema => schema.name
        )),
        [schemas]
    )

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
                    sortedSchemas.map(schema => 
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