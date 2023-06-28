const { app, BrowserWindow } = require('electron')
const path = require('path')

try {
  require('electron-reloader')(module)
} catch (_) {}

const createWindow = () => {
    const appWindow = new BrowserWindow({
      width: 1024,
      height: 768,
      webPreferences: {
        preload: path.join(__dirname, 'preload.js')
      }
    })
  
    appWindow.loadFile('index.html')
}

app.whenReady().then(() => {
    createWindow()
})

app.on('window-all-closed', () => {
    app.quit()
})
