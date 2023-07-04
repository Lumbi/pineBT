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
                const path = result.filePaths[0]
                if (result.canceled || !path) { return }
                const data = await fs.readFile(path, 'utf8')
                const document = { path, data }
                appWindow.webContents.send('menu/file/open', document)
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

    ipcMain.on('set-title', (_, title) => {
      appWindow.setTitle(title)
    })

    ipcMain.handle('save-file', async (_, file) => {
      let filePath = file.path
      if (!filePath) {
        const result = await dialog.showSaveDialog({
          defaultPath: 'untitled.json',
          filters: [ { name: 'JSON', extensions: ['json'] } ],
        })
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
