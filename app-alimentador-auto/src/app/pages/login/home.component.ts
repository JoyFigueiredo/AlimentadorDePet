import { Component } from '@angular/core';
import { DefaultloginLayoutComponent } from '../../components/default-login-layout/default-login-layout.component';
import { FormControl, FormGroup, ReactiveFormsModule, Validators } from '@angular/forms';
import { Router } from '@angular/router';

@Component({
  selector: 'app-login',
  standalone: true,
  imports: [
    DefaultloginLayoutComponent,
    ReactiveFormsModule
  ],
  templateUrl: './login.component.html',
  styleUrls: ['./login.component.css']
})
export class loginComponent {
  loginForm!: FormGroup;

  constructor(private router: Router) {
    this.loginForm = new FormGroup({
      qntVolta: new FormControl('', [Validators.required, Validators.min(1)]),
      data: new FormControl('', Validators.required),
      hora: new FormControl('', Validators.required)
    })
  }


  irParaAlimentar() {
    console.log('apertou');
    this.router.navigate(['/alimentar']);
  }

}

