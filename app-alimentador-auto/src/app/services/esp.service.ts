import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { Observable } from 'rxjs';
import { environment } from '../pages/environments/environment';

export interface Alimentacao {
  id?: string;
  qnt: string;
  dataHora?: string;
}

@Injectable({ providedIn: 'root' })
export class EspService {
  private apiUrl = environment.apiUrl; // http://192.168.2.114:9090
  private espUrl = 'http://192.168.2.117/alimentar';

  constructor(private http: HttpClient) {}

  /** Envia comando para ESP */
  alimentar(quantidade: number): Observable<any> {
    const params = { quantidade: quantidade.toString() };
    return this.http.get(this.espUrl, { params, responseType: 'text' });
  }

  /** Salva histórico no backend */
  salvarHistoricoBackend(quantidade: number): Observable<Alimentacao> {
    const historico = { qnt: quantidade.toString() };
    return this.http.post<Alimentacao>(`${this.apiUrl}/historico`, historico);
  }

  /** Obtém histórico do backend */
  obterHistoricoBackend(): Observable<Alimentacao[]> {
    return this.http.get<Alimentacao[]>(`${this.apiUrl}/historico`);
  }

  /** Reseta histórico no backend */
  resetarHistoricoBackend(): Observable<void> {
    return this.http.delete<void>(`${this.apiUrl}/historico`);
  }
}
