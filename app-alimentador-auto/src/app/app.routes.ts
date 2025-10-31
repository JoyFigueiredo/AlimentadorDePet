import { Routes } from '@angular/router';
import { loginComponent } from './pages/login/home.component';
import { AlimentarComponent } from './pages/alimentar/alimentar.component';
import { HistoricoComponent } from './pages/historico/historico.component';


export const routes: Routes = [
  {path: '', redirectTo: 'login', pathMatch:'full'},
  {path: "login", component: loginComponent },
  {path: "alimentar", component: AlimentarComponent },
  {path: "historico", component: HistoricoComponent }
];


