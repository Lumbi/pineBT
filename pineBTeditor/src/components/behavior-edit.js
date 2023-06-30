import Offcanvas from 'react-bootstrap/Offcanvas'
import Stack from 'react-bootstrap/Stack'
import ToggleButton from 'react-bootstrap/ToggleButton'
import Button from 'react-bootstrap/Button'
import Dropdown from 'react-bootstrap/Dropdown'
import bem from '../bem'

import './behavior-edit.less'

function BehaviorEditOption(props) {
    const { option, onChange } = props
    const { key, type, value } = option
    if (type === 'boolean') {
        return (
            <ToggleButton
                id={`behavior-edit__${option.key}`}
                type='checkbox'
                variant='outline-primary'
                checked={!!value}
                onChange={(event) =>
                    onChange({
                        ...option,
                        value: event.currentTarget.checked
                    })
                }
            >
                {key}
            </ToggleButton>
        )
    } else if (type === 'number') {
        return <p>Not supported yet</p>
    } else if (typeof type === 'number') { // enumeration case
        const count = type
        function caseName(value) { return `${key}_${value}` }
        const cases = [...Array(count).keys()]
        return (
            <Dropdown>
                <Dropdown.Toggle>
                {
                    value !== undefined
                        ? caseName(value)
                        : caseName(0)
                }
                </Dropdown.Toggle>
                <Dropdown.Menu>
                    {
                        cases.map(value =>
                            <Dropdown.Item
                                key={value}
                                onClick={() => onChange({
                                    ...option,
                                    value: value
                                })}
                            >
                                {caseName(value)}
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
        onHide,
        onEdit,
        onDelete,
    } = props

    const title = (behavior && behavior.schema) || ''
    const options = schema && schema.options && Object.entries(schema.options) || []

    function valueForOptionKey(key) {
        if (!behavior || !behavior.options) { return undefined }
        return behavior.options[key]
    }

    function updatedBehaviorWithOption(option) {
        return {
            ...behavior,
            options: {
                ...behavior.options,
                [option.key]: option.value
            }
        }
    }

    return (
        <Offcanvas id='behavior-edit' show={show} onHide={onHide} placement='end'>
            <Offcanvas.Header closeButton>
                <Offcanvas.Title>{title}</Offcanvas.Title>
            </Offcanvas.Header>
            <Offcanvas.Body>
                <Stack id={bem('behavior-edit', 'list')} gap={3}>
                {
                    options.map(pair => {
                        const [key, type] = pair
                        return (
                            <BehaviorEditOption
                                key={key}
                                option={{ 
                                    key,
                                    type,
                                    value: valueForOptionKey(key)
                                }}
                                onChange={(option) => {
                                    onEdit(updatedBehaviorWithOption(option))
                                }}
                            />
                        )
                    })
                }
                <Button
                    id={bem('behavior-edit', 'delete-button')}
                    variant='danger'
                    onClick={() => onDelete(behavior)}
                >
                    Delete
                </Button>
                </Stack>
            </Offcanvas.Body>
        </Offcanvas>
    )
}