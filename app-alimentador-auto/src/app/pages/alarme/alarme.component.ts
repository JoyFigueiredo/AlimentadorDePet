import { Component, OnInit } from '@angular/core';
import { CommonModule } from '@angular/common';
import { FormsModule } from '@angular/forms';
import { EspService } from '../../services/esp.service';

@Component({
  selector: 'app-alarme',
  standalone: true,
  imports: [CommonModule, FormsModule],
  templateUrl: './alarme.component.html',
  styleUrls: ['./alarme.component.css'],
})
export class AlarmeComponent implements OnInit {
  hora: number = 0;
  minuto: number = 0;
  quantidade: number = 0.1;

  alarmes: any[] = [];

  constructor(private espService: EspService) {}

  ngOnInit() {
    this.carregarAgendamentos();
  }

  carregarAgendamentos() {
    this.espService.obterAgendamentos().subscribe((dados) => {
      this.alarmes = dados;
    });
  }

  adicionar() {
    const novo = {
      hora: this.hora,
      minuto: this.minuto,
      quantidade: this.quantidade,
    };

    this.espService.salvarAgendamento(novo).subscribe(() => {
      this.carregarAgendamentos();
    });
  }

  remover(id: string) {
    this.espService.deletarAgendamento(id).subscribe(() => {
      this.carregarAgendamentos();
    });
  }

  aumentar() {
    if (this.quantidade < 2) {
      this.quantidade = parseFloat((this.quantidade + 0.1).toFixed(1));
    }
  }

  diminuir() {
    if (this.quantidade > 0.1) {
      this.quantidade = parseFloat((this.quantidade - 0.1).toFixed(1));
    }
  }
}
