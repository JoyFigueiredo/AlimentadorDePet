import { Component, OnInit } from '@angular/core';
import { CommonModule } from '@angular/common';
import { EspService, Alimentacao } from '../../services/esp.service';

@Component({
  selector: 'app-historico',
  standalone: true,
  imports: [CommonModule],
  templateUrl: './historico.component.html',
  styleUrls: ['./historico.component.css']
})
export class HistoricoComponent implements OnInit {
  historico: Alimentacao[] = [];
  carregando = false;

  constructor(private espService: EspService) { }

  ngOnInit() {
    this.carregarHistorico();
  }

  carregarHistorico() {
    this.carregando = true;
    this.espService.obterHistorico().subscribe({
      next: (dados) => {
        this.historico = dados;
        this.carregando = false;
      },
      error: (err) => {
        console.error('Erro:', err);
        this.carregando = false;
      }
    });
  }

  resetar() {
    if (confirm('Tem certeza que deseja limpar o histórico?')) {
      this.espService.resetarHistorico().subscribe({
        next: () => {
          this.historico = [];
          alert('Histórico resetado com sucesso!');
        },
        error: (err) => console.error('Erro ao resetar histórico:', err)
      });
    }
  }
}
