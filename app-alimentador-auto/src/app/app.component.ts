import { Component, OnInit } from '@angular/core';
import { CommonModule } from '@angular/common';
import { FormsModule } from '@angular/forms';
import { RouterOutlet } from '@angular/router';
import { HeaderComponent } from './components/header/header.component';
import { FooterComponent } from './components/footer/footer.component';
import { EspService, Alimentacao } from './services/esp.service';

@Component({
  selector: 'app-root',
  standalone: true,
  imports: [
    CommonModule,
    FormsModule,
    RouterOutlet,
    HeaderComponent,
    FooterComponent,
  ],
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.css'],
})
export class AppComponent implements OnInit {
  quantidade: string = '0.1';
  historico: Alimentacao[] = [];
  carregando = false;

  constructor(private espService: EspService) {}

  ngOnInit(): void {
    this.carregarHistorico();
  }

  alimentar(): void {
    this.carregando = true;
    this.espService.alimentar(parseFloat(this.quantidade)).subscribe({
      next: () => {
        this.carregando = false;
        alert(`🐾 Alimentação de ${this.quantidade}g simulada!`);
        this.salvarHistorico();
      },
      error: (err: any) => {
        this.carregando = false;
        console.error('Erro ao enviar comando para ESP:', err);
      },
    });
  }

  salvarHistorico(): void {
    this.espService
      .salvarHistoricoBackend(parseFloat(this.quantidade))
      .subscribe({
        next: () => this.carregarHistorico(),
        error: (err: any) =>
          console.error('Erro ao salvar histórico no backend:', err),
      });
  }

  carregarHistorico(): void {
    this.espService.obterHistoricoBackend().subscribe({
      next: (dados: Alimentacao[]) => (this.historico = dados),
      error: (err: any) =>
        console.error('Erro ao carregar histórico do backend:', err),
    });
  }

  resetarHistorico(): void {
    if (confirm('Tem certeza que deseja limpar o histórico?')) {
      this.espService.resetarHistoricoBackend().subscribe({
        next: () => {
          this.historico = [];
          alert('Histórico resetado!');
        },
        error: (err: any) =>
          console.error('Erro ao resetar histórico no backend:', err),
      });
    }
  }
}
