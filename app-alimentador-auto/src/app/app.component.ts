import { Component } from '@angular/core';
import { HttpClient, HttpClientModule } from '@angular/common/http';
import { RouterOutlet } from '@angular/router';
import { CommonModule } from '@angular/common';
import { HeaderComponent } from './components/header/header.component';
import { FooterComponent } from './components/footer/footer.component';

@Component({
  selector: 'app-root',
  standalone: true,
  imports: [
    RouterOutlet,
    CommonModule,
    HeaderComponent,
    FooterComponent
  ],
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.css']
})
export class AppComponent {
  title = '🐾 Alimentador Automático';
  menuSelecionado: string | null = null;
  lcdText = 'Pronto';
  horario: string | null = null;

  private espUrl = 'http://192.168.1.50'; // IP fixo do ESP

  constructor(private http: HttpClient) { }

  selecionarMenu(menu: string) {
    this.menuSelecionado = menu;
    this.lcdText = `Menu: ${menu}`;

    if (menu === 'horario') {
      this.obterHorario();
    }
  }

  enviarAcao(acao: string) {
    this.http.get(`${this.espUrl}/${acao}`).subscribe({
      next: (res: any) => {
        this.lcdText = `Executando: ${acao}`;
        console.log('Comando enviado:', res);
      },
      error: (err) => {
        this.lcdText = 'Erro de conexão';
        console.error(err);
      }
    });
  }

  obterHorario() {
    this.http.get(`${this.espUrl}/hora`).subscribe({
      next: (res: any) => {
        this.horario = res.hora;
        this.lcdText = `Hora: ${res.hora}`;
      },
      error: () => {
        this.lcdText = 'Erro ao obter hora';
      }
    });
  }
}
