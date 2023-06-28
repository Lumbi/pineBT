export default function bem(block, element, modifiers) {
    var name = block
    if (element) {
        name += ('__' + element)
    }

    var classNames = [name]

    if (modifiers) {
        for (const [modifier, value] of Object.entries(modifiers)) {
            if (value) {
                classNames.push(name + '--' + modifier)
            }
        }
    }

    return classNames.join(' ')
}
