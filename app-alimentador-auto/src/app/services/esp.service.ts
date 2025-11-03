import { Injectable } from '@angular/core';
import { Observable, of, delay } from 'rxjs';

export interface Alimentacao {
  id?: number;
  dataHora: string;
  quantidade: number;
}

@Injectable({
  providedIn: 'root'
})
export class EspService {
  private storageKey = 'historicoAlimentacao';

  constructor() { }

  /** Simula enviar o comando de alimentação */
  alimentar(quantidade: number): Observable<any> {
    console.log(`Simulando envio de ${quantidade}g ao ESP...`);
    return of({ sucesso: true }).pipe(delay(500)); // Simula 0.5s de atraso
  }

  /** Salva o registro localmente */
  salvarHistorico(quantidade: number): Observable<any> {
    const historico = this.obterHistoricoLocal();
    const novoRegistro = {
      id: Date.now(),
      dataHora: new Date().toLocaleString(),
      quantidade
    };
    historico.unshift(novoRegistro); // adiciona no início
    localStorage.setItem(this.storageKey, JSON.stringify(historico));
    return of(novoRegistro).pipe(delay(300));
  }

  /** Obtém o histórico salvo localmente */
  obterHistorico(): Observable<Alimentacao[]> {
    return of(this.obterHistoricoLocal()).pipe(delay(200));
  }

  private obterHistoricoLocal(): Alimentacao[] {
    const dados = localStorage.getItem(this.storageKey);
    return dados ? JSON.parse(dados) : [];
  }

  /** Reseta o histórico local */
  resetarHistorico(): Observable<void> {
    localStorage.removeItem(this.storageKey);
    return of(void 0).pipe(delay(200));
  }
}
