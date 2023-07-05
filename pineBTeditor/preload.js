const { contextBridge, ipcRenderer } = require('electron')
const ffi = require('ffi-napi')

function subscribe(channel) {
  return (callback) => {
    ipcRenderer.on(channel, callback)
    return () => ipcRenderer.removeListener(channel, callback)
  }
} 

contextBridge.exposeInMainWorld('electron', {
  setTitle: (title) => ipcRenderer.send('set-title', title),
  saveFile: (file) => ipcRenderer.invoke('save-file', file),
})

contextBridge.exposeInMainWorld('menu', {
  file: {
    new: subscribe('menu/file/new'),
    open: subscribe('menu/file/open'),
    save: subscribe('menu/file/save'),
  }
})

const pineBT = ffi.Library('pineBT', {
  pineBT_schemas: [ 'void', [ 'char *' ] ],
  pineBT_create: [ 'int', [ 'string' ] ],
  pineBT_destroy: [ 'void', [ 'int' ] ],
  pineBT_run: [ 'void', [ 'int' ] ],
  pineBT_status: [ 'void', [ 'int', 'char *' ] ],
})

function stringFromBuffer(buffer) {
  var result = buffer.toString('utf-8')
  var nullTerminator = result.indexOf('\u0000')
  if (nullTerminator >= 0) { 
    result = result.substring(0, nullTerminator)
  }
  return result
}

contextBridge.exposeInMainWorld('pineBT', {
  schemas: () => {
    const buffer = Buffer.alloc(5000, 0) // TODO: Use a sensible value for the buffer?
    pineBT.pineBT_schemas(buffer)
    return JSON.parse(stringFromBuffer(buffer))
  },

  create: () => {
    return pineBT.pineBT_create()
  },

  destroy: (handle) => {
    return pineBT.pineBT_destroy(handle)
  },

  run: (handle) => {
    return pineBT.pineBT_run(handle)
  },

  status: (handle) => {
    const buffer = Buffer.alloc(5000, 0) // TODO: Use a sensible value for the buffer?
    pineBT.pineBT_status(handle, buffer)
    return JSON.parse(stringFromBuffer(buffer))
  }
})
