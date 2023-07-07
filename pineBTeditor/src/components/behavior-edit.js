import Offcanvas from 'react-bootstrap/Offcanvas'
import Stack from 'react-bootstrap/Stack'
import ToggleButton from 'react-bootstrap/ToggleButton'
import Button from 'react-bootstrap/Button'
import Dropdown from 'react-bootstrap/Dropdown'
import Form from 'react-bootstrap/Form'
import { t } from 'i18next'
import bem from '../bem'
import { updatedBehaviorWithOption } from '../models/behavior'
import * as Document from '../models/document'
import { byPreferredOrder } from '../sort'

import './behavior-edit.less'

const preferredOptionsOrder = [
    'mode',
    'key',
    'negate',
    'predicate',
    'target',
]

export default function BehaviorEdit(props) {
    const {
        behavior,
        show,
        document,
        onHide,
    } = props

    const title = (behavior && behavior.schema) || ''
    const schema = Document.schemaForBehavior(document, behavior)
    const options = (schema && schema.options && Object.entries(schema.options) || [])
        .sort(byPreferredOrder(preferredOptionsOrder, option => option[0]))

    function valueForOptionKey(key) {
        if (!behavior || !behavior.options) { return undefined }
        return behavior.options[key]
    }

    function handleOnEditOption(option) {
        Document.updateBehavior(
            document, 
            updatedBehaviorWithOption(behavior, option)
        )
    }

    function handleOnDeleteBehavior(behavior) {
        onHide()
        setTimeout(() => {
            Document.deleteBehaviorById(document, behavior.id)
        }, 300)
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
                                schema={schema}
                                option={{ 
                                    key,
                                    type,
                                    value: valueForOptionKey(key)
                                }}
                                onChange={(option) => handleOnEditOption(option)}
                            />
                        )
                    })
                }
                    <Button
                        id={bem('behavior-edit', 'delete-button')}
                        variant='danger'
                        onClick={() => handleOnDeleteBehavior(behavior)}
                    >
                        Delete
                    </Button>
                </Stack>
            </Offcanvas.Body>
        </Offcanvas>
    )
}

function BehaviorEditOption(props) {
    const { option, schema, onChange } = props
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
                {t(`${schema.name}_${key}_${!!value}`)}
            </ToggleButton>
        )
    } else if (type === 'number') {
        const id = `${schema.name}_${option.key}`
        return (
            <Stack direction='horizontal' gap={2}>
                <Form.Label htmlFor={id}>{t(id)}</Form.Label>
                <Form.Control 
                    id={id} 
                    type="number" 
                    value={isNaN(option.value) ? '' : option.value}
                    onChange={(event) => 
                        onChange({
                            ...option,
                            value: isNaN(event.currentTarget.value) ? undefined : event.currentTarget.value,
                        })
                    }
                />
            </Stack>
        )
    } else if (typeof type === 'number') { // enumeration case
        const count = type
        const id = `${schema.name}_${key}`
        function caseName(value) { return t(`${schema.name}_${key}_${value}`) }
        const cases = [...Array(count).keys()]
        return (
            <Stack direction='horizontal' gap={2}>
                <Form.Label htmlFor={id}>{t(id)}</Form.Label>
                <Dropdown>
                    <Dropdown.Toggle id={id} style={{ verticalAlign: 'baseline' }}>
                    {
                        value && value.case
                            ? caseName(value.case)
                            : caseName(0)
                    }
                    </Dropdown.Toggle>
                    <Dropdown.Menu>
                        {
                            cases.map(c =>
                                <Dropdown.Item
                                    key={c}
                                    onClick={() => onChange({
                                        ...option,
                                        value: { case: c }
                                    })}
                                >
                                    {caseName(c)}
                                </Dropdown.Item>
                            )
                        }
                    </Dropdown.Menu>
                </Dropdown>
            </Stack>
        )
    }
    return null
}
