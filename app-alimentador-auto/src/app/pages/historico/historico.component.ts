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

  constructor(private espService: EspService) { }

  ngOnInit() {
    this.historico = this.espService.obterHistorico();
  }

  resetar() {
    this.espService.resetarHistorico();
    this.historico = [];
  }
}
