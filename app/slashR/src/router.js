import { createRouter, createWebHashHistory } from 'vue-router';

import Launcher from './components/Launcher.vue'
import Workspace from './components/Workspace.vue'
import Loading from './components/Loading.vue'
const routes = [
    { path: '/', component: Loading, name: 'loading' },
    { path: '/launcher', component: Launcher, name: 'launcher' },
    { path: '/workspace/:projectName', component: Workspace, name: 'workspace'},
  ];

  const router = createRouter({
    history: createWebHashHistory(),
    routes,
  });

  export default router;