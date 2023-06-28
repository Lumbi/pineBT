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
        const behaviorCard = document.getElementById(`behavior-card-${from.id}`)
        const { width, height } = behaviorCard.getBoundingClientRect()
        setFromAnchor({ 
            x: from.position.x + width / 2,
            y: from.position.y + height - 8
        })
        setToAnchor({
            x: to.position.x + width / 2,
            y: to.position.y + 8
        })
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