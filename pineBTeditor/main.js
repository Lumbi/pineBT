const { app, BrowserWindow, Menu, dialog, ipcMain } = require('electron')
const path = require('path')
const fs = require('fs').promises

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

    return appWindow
}

app.whenReady().then(() => {
    const appWindow = createWindow()

    Menu.setApplicationMenu(
      Menu.buildFromTemplate([
        {
          label: '&File',
          submenu: [
            {
              label: 'New',
              accelerator: 'Ctrl+N',
              click: () => appWindow.webContents.send('menu/file/new'),
            },
            { type: 'separator' },
            { 
              label: 'Open...',
              accelerator: 'Ctrl+O',
              click: async () => {
                const result = await dialog.showOpenDialog({ 
                  properties: ['openFile'],
                  filters: [ { name: 'JSON', extensions: ['json'] } ],
                })
                const filePath = result.filePaths[0]
                if (!result.canceled) {
                  console.log(filePath)
                }
              },
            },
            {
              label: 'Save',
              accelerator: 'Ctrl+S',
              click: () => appWindow.webContents.send('menu/file/save'),
            },
            {
              label: 'Save as...',
              accelerator: 'Ctrl+Shift+S',
              click: () => {},
            },
            { type: 'separator' },
            { role: 'quit' },
          ]
        }
      ])
    )

    appWindow.webContents.openDevTools()

    ipcMain.handle('save-file', async (_, file) => {
      let filePath = file.path
      if (!filePath) {
        const result = await dialog.showSaveDialog()
        filePath = result.filePath
      }
      if (filePath) {
        await fs.writeFile(filePath, file.data)
        return { path: filePath }
      }
      return {}
    })
})

app.on('window-all-closed', () => {
    app.quit()
})
