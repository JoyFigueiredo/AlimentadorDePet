import { Component } from '@angular/core';

import {
  FormControl,
  FormGroup,
  ReactiveFormsModule,
  Validators,
} from '@angular/forms';
import { Router } from '@angular/router';
import { DefaultinicioLayoutComponent } from '../../components/default-inicio-layout/default-inicio-layout.component';

@Component({
  selector: 'app-inicio',
  standalone: true,
  imports: [DefaultinicioLayoutComponent, ReactiveFormsModule],
  templateUrl: './inicio.component.html',
  styleUrls: ['./inicio.component.css'],
})
export class InicioComponent {
  inicioForm!: FormGroup;

  constructor(private router: Router) {
    this.inicioForm = new FormGroup({
      qntVolta: new FormControl('', [Validators.required, Validators.min(1)]),
      data: new FormControl('', Validators.required),
      hora: new FormControl('', Validators.required),
    });
  }

  irParaAlimentar() {
    console.log('apertou');
    this.router.navigate(['/alimentar']);
  }

  irParaHistorico() {
    console.log('apertou');
    this.router.navigate(['/historico']);
  }
}
