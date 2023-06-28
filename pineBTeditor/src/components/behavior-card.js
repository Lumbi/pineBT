import { useState, useRef, useEffect } from 'react'

import './behavior-card.less'

export default function BehaviorCard(props) {
    const behavior = props.behavior

    return <div 
        key={behavior.id}
        className='behavior-card'
        style={
            {
                position: 'relative',
                left: behavior.position.x,
                top: behavior.position.y,
                width: 300,
                height: 100,
            }
        }
    >
        <p>{behavior.schema}</p>
        <div className='behavior-card__handle'></div>
    </div>
}
