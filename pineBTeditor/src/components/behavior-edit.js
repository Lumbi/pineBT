import Offcanvas from 'react-bootstrap/Offcanvas'
import Stack from 'react-bootstrap/Stack'
import ToggleButton from 'react-bootstrap/ToggleButton'
import Dropdown from 'react-bootstrap/Dropdown'

function BehaviorEditOption(props) {
    const {
        key,
        type,
    } = props.option
    if (type === 'boolean') {
        return (
            <ToggleButton
                type='checkbox'
            >
                {key}
            </ToggleButton>
        )
    } else if (type === 'number') {
        return <p>Not supported yet</p>
    } else if (typeof type === 'number') { // enumeration case
        const count = type
        const cases = [...Array(count).keys()]
            .map(n => n + 1)
            .map(i => ({ name: `${key}_${i}` }))
        return (
            <Dropdown>
                <Dropdown.Toggle>{key}</Dropdown.Toggle>
                <Dropdown.Menu>
                    {
                        cases.map(c => 
                            <Dropdown.Item key={c.name}>
                                {c.name}
                            </Dropdown.Item>
                        )
                    }
                </Dropdown.Menu>
            </Dropdown>
        )
    }
    return null
}

export function BehaviorEdit(props) {
    const {
        behavior,
        schema,
        show,
        onHide
    } = props

    const title = (behavior && behavior.schema) || ''
    const options = schema && schema.options && Object.entries(schema.options) || []

    return (
        <Offcanvas show={show} onHide={onHide} placement='end'>
            <Offcanvas.Header closeButton>
                <Offcanvas.Title>{title}</Offcanvas.Title>
            </Offcanvas.Header>
            <Offcanvas.Body>
                <Stack gap={3}>
                {
                    options.map(option => {
                        const [key, type] = option
                        return (
                            <BehaviorEditOption
                                key={key}
                                option={{ key, type }}
                                type={type}
                            />
                        )
                    })
                }
                </Stack>
            </Offcanvas.Body>
        </Offcanvas>
    )
}
