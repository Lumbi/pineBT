export function reset(editor) {
    editor.setScrollOffset({ x: 0, y: 0 })
}

export function beginNewConnection(editor, from) {
    editor.setNewConnection({ from })
}

export function endNewConnection(editor) {
    editor.setNewConnection(undefined)
}
