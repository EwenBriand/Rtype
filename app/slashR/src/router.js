import { createRouter, createWebHashHistory } from 'vue-router';

import Launcher from './components/Launcher.vue'
import Workspace from './components/Workspace.vue'
import Loading from './components/Loading.vue'
const routes = [
    { path: '/', component: Loading },
    { path: '/launcher', component: Launcher },
    { path: '/workspace', component: Workspace },
  ];

  const router = createRouter({
    history: createWebHashHistory(),
    routes,
  });

  export default router;