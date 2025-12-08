import { Routes } from '@angular/router';
import { InicioComponent } from './pages/inicio/inicio.component';
import { AlimentarComponent } from './pages/alimentar/alimentar.component';
import { HistoricoComponent } from './pages/historico/historico.component';
import { AlarmeComponent } from './pages/alarme/alarme.component';

export const routes: Routes = [
  { path: '', redirectTo: 'inicio', pathMatch: 'full' },
  { path: 'inicio', component: InicioComponent },
  { path: 'alimentar', component: AlimentarComponent },
  { path: 'historico', component: HistoricoComponent },
  { path: 'alarme', component: AlarmeComponent },
];
