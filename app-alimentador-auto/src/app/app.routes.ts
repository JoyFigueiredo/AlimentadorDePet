import { Routes } from '@angular/router';
import { LoginComponent } from './pages/login/login.component';
import { AlimentarComponent } from './pages/alimentar/alimentar.component';
import { HistoricoComponent } from './pages/historico/historico.component';


export const routes: Routes = [
  {path: '', redirectTo: 'login', pathMatch:'full'},
  {path: "login", component: LoginComponent },
  {path: "alimentar", component: AlimentarComponent },
  {path: "historico", component: HistoricoComponent }
];


