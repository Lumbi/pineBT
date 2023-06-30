import { createRoot } from 'react-dom/client'
import i18next from 'i18next'
import App from './components/app'

import 'bootstrap-icons/font/bootstrap-icons.css'
import './renderer.less'

i18next.init({
    lng: 'en',
    debug: true, // TODO: Enable only in dev
    resources: {
      en: require('./localization/en.json')
    }
}).then(() => {
    document.body.innerHTML = '<div id="app"></div>'
    const root = createRoot(document.getElementById('app'));
    root.render(<App />);
})

// const btBlueprint = `
// {
//     "root" : {
//         "schema" : "Selector",
//         "id" : 1,
//         "options" : {
//             "live" : true
//         },
//         "children" : [
//             { 
//                 "schema" : "MockCondition", 
//                 "id" : 11,
//                 "children" : [ { 
//                     "schema" : "MockTask",
//                     "id" : 111
//                 } ] 
//             },
//             {
//                 "schema" : "Parallel",
//                 "id" : 12,
//                 "children" : [
//                     { "schema" : "MockTask", "id" : 121 },
//                     { "schema" : "MockTask", "id" : 122 }
//                 ]
//             }
//         ]
//     }
// }
// `
//
// const run = document.getElementById('run')
// const blueprint = document.getElementById('blueprint')
// const results = document.getElementById('results')

// function log(message) {
//     results.innerText += (message + "\n")
// }

// blueprint.innerText = btBlueprint

// const handle = pineBT.create(btBlueprint)
// log(`BT# ${handle}`)

// run.addEventListener('click', () => {
//     try {
//         pineBT.run(handle);
//         const status = pineBT.status(handle)
//         log(JSON.stringify(status, null, 2))
//     } catch (error) {
//         log('failed to parse')
//         console.log(error)
//     }
// })
