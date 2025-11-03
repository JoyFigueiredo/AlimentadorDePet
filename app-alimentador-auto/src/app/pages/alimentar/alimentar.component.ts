import { Component } from '@angular/core';
import { CommonModule } from '@angular/common';
import { FormsModule } from '@angular/forms';
import { EspService } from '../../services/esp.service';

@Component({
  selector: 'app-alimentar',
  standalone: true,
  imports: [CommonModule, FormsModule],
  templateUrl: './alimentar.component.html',
  styleUrls: ['./alimentar.component.css']
})
export class AlimentarComponent {
  quantidade: number = 0.1;

  constructor(private espService: EspService) { }

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

  enviar() {
    this.espService.alimentar(this.quantidade).subscribe({
      next: () => {
        alert(`Simulação: ${this.quantidade}g enviadas!`);
        this.espService.salvarHistorico(this.quantidade).subscribe();
      },
      error: (err) => {
        console.error('Erro:', err);
      }
    });
  }
}
