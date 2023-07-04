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
