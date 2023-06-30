console.log('preload.js')

const { contextBridge } = require('electron')

contextBridge.exposeInMainWorld('versions', {
  node: () => process.versions.node,
  chrome: () => process.versions.chrome,
  electron: () => process.versions.electron
})

const ffi = require('ffi-napi')

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
