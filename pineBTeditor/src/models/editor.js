export function reset(editor) {
    editor.setScrollOffset({ x: 0, y: 0 })
}

export function beginNewConnection(editor, from) {
    editor.setNewConnection({ from })
}

export function endNewConnection(editor) {
    editor.setNewConnection(undefined)
}

export function inEditBehavior(editor) {
    const { document, inEditBehaviorId } = editor
    return document.behaviors.find(b => b.id === inEditBehaviorId)
}

export function beginEditBehavior(editor, behavior) {
    editor.setInEditBehaviorId(behavior.id)
    editor.setShowBehaviorEdit(true)
}

export function endEditBehavior(editor) {
    editor.setShowBehaviorEdit(false)
}
