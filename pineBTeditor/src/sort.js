export function numericallyAscending(a ,b) {
    return a - b
}

export function byPreferredOrder(preferredOrder, key) {
    return ((a, b) => {
        const aOrder = preferredOrder.indexOf((key && key(a)) || a)
        const bOrder = preferredOrder.indexOf((key && key(b)) || b)
        if (aOrder >= 0 && bOrder >= 0) { return aOrder - bOrder }
        if (aOrder >= 0) { return -1 }
        if (bOrder >= 0) { return 1 }
        return a - b
    })
}
