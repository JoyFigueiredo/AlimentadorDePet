import { Component, OnInit } from '@angular/core';
import { CommonModule } from '@angular/common';
import { EspService, Alimentacao } from '../../services/esp.service';

@Component({
  selector: 'app-historico',
  standalone: true,
  imports: [CommonModule],
  templateUrl: './historico.component.html',
  styleUrls: ['./historico.component.css'],
})
export class HistoricoComponent implements OnInit {
  historico: Alimentacao[] = [];
  carregando = false;

  constructor(private espService: EspService) {}

  ngOnInit() {
    this.carregarHistorico();
  }

  carregarHistorico() {
    this.carregando = true;
    this.espService.obterHistoricoBackend().subscribe({
      next: (dados) => {
        this.historico = dados
          .sort((a, b) =>
            a.dataHora && b.dataHora ? (a.dataHora < b.dataHora ? 1 : -1) : 0
          )
          .slice(0, 20);
        this.carregando = false;
      },
      error: (err) => {
        console.error('Erro ao carregar histórico:', err);
        this.carregando = false;
      },
    });
  }

  resetar() {
    if (confirm('Tem certeza que deseja limpar o histórico?')) {
      this.espService.resetarHistoricoBackend().subscribe({
        next: () => {
          this.historico = [];
          alert('Histórico resetado com sucesso!');
        },
        error: (err) => {
          console.error('Erro ao resetar histórico:', err);
          alert('Erro ao resetar histórico.');
        },
      });
    }
  }
}
