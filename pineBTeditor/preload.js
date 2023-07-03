const { contextBridge, ipcRenderer } = require('electron')
const ffi = require('ffi-napi')

function subscribe(channel) {
  return (callback) => {
    ipcRenderer.on(channel, callback)
    return () => ipcRenderer.removeListener(channel, callback)
  }
} 

contextBridge.exposeInMainWorld('electron', {
  saveFile: (file) => ipcRenderer.invoke('save-file', file),
})

contextBridge.exposeInMainWorld('menu', {
  on: {
    file: {
      save: subscribe('menu/file/save'),
    }
  }
})

const pineBT = ffi.Library('pineBT', {
  pineBT_create: [ 'int', [ 'string' ] ],
  pineBT_destroy: [ 'void', [ 'int' ] ],
  pineBT_run: [ 'void', [ 'int' ] ],
  pineBT_status: [ 'void', [ 'int', 'char *' ] ],
})

contextBridge.exposeInMainWorld('pineBT', {
  create: pineBT.pineBT_create,

  destroy: pineBT.pineBT_destroy,

  run: pineBT.pineBT_run,

  status: (handle) => {
    const buffer = Buffer.alloc(5000, 0) // TODO: Use a sensible value for the buffer?
    pineBT.pineBT_status(handle, buffer)
    var result = buffer.toString('utf-8')
    var nullTerminator = result.indexOf('\u0000')
    if (nullTerminator >= 0) { 
      result = result.substring(0, nullTerminator)
    }
    return JSON.parse(result)
  }
})
