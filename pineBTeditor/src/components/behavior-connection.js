import { useEffect, useState } from 'react'

import bem from '../bem'

import './behavior-connection.less'

export default function BehaviorConnection(props) {
    const {
        from,
        to
    } = props

    const [fromAnchor, setFromAnchor] = useState({ x: from.position.x, y: from.position.y })
    const [toAnchor, setToAnchor] = useState({ x: to.position.x, y: to.position.y })

    useEffect(() => {
        const fromBehaviorCard = document.getElementById(`behavior-card-${from.id}`)
        const fromSize = fromBehaviorCard.getBoundingClientRect()
        setFromAnchor({ 
            x: from.position.x + fromSize.width / 2,
            y: from.position.y + fromSize.height - 8
        })

        if (to && to.id) {
            const toBehaviorCard = document.getElementById(`behavior-card-${from.id}`)
            const toSize = toBehaviorCard.getBoundingClientRect()
            setToAnchor({
                x: to.position.x + toSize.width / 2,
                y: to.position.y + 8
            })
        } else {
            setToAnchor(to.position)
        }
    }, [from.position, to.position])

    return (
        <line 
            className={bem('behavior-connection', 'line')}
            x1={fromAnchor.x}
            y1={fromAnchor.y} 
            x2={toAnchor.x} 
            y2={toAnchor.y}
        />
    )
}