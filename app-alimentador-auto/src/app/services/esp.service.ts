import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { Observable } from 'rxjs';

export interface Alimentacao {
  dataHora: string;
  quantidade: number;
}

@Injectable({
  providedIn: 'root'
})
export class EspService {
  private espUrl = 'http://192.168.1.50'; // IP do ESP
  private storageKey = 'historicoAlimentacoes';

  constructor(private http: HttpClient) { }

  // Envia a alimentação para o ESP
  alimentar(quantidade: number): Observable<any> {
    return this.http.get(`${this.espUrl}/alimentar?quantidade=${quantidade}`);
  }

  // Salva no histórico local (localStorage)
  salvarHistorico(quantidade: number) {
    const historico = this.obterHistorico();
    historico.unshift({
      dataHora: new Date().toISOString(),
      quantidade
    });
    if (historico.length > 20) historico.pop();
    localStorage.setItem(this.storageKey, JSON.stringify(historico));
  }

  // Obtém o histórico
  obterHistorico(): Alimentacao[] {
    const data = localStorage.getItem(this.storageKey);
    return data ? JSON.parse(data) : [];
  }

  // Limpa o histórico
  resetarHistorico() {
    localStorage.removeItem(this.storageKey);
  }
}
