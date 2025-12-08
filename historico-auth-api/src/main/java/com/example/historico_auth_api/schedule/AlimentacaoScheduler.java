package com.example.historico_auth_api.schedule;

import org.springframework.scheduling.annotation.Scheduled;
import org.springframework.stereotype.Component;

import com.example.historico_auth_api.domain.Agendamento;
import com.example.historico_auth_api.service.AgendamentoService;
import com.example.historico_auth_api.service.HistoricoService;

import java.time.LocalDateTime;
import java.util.HashSet;
import java.util.Set;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.client.RestTemplate;

@Component
public class AlimentacaoScheduler {

    @Autowired
    private AgendamentoService agendamentoService;

    @Autowired
    private HistoricoService historicoService;

    // Evita executar mais de 1 vez no mesmo minuto
    private Set<String> executadosHoje = new HashSet<>();

    @Scheduled(fixedRate = 60000) // roda a cada 60s
    public void verificarAgendamentos() {
        LocalDateTime agora = LocalDateTime.now();
        String chaveDia = agora.toLocalDate().toString();

        // Reset diário da lista de execuções
        if (!executadosHoje.contains(chaveDia)) {
            executadosHoje.clear();
            executadosHoje.add(chaveDia);
        }

        int hora = agora.getHour();
        int minuto = agora.getMinute();

        for (Agendamento ag : agendamentoService.listarTodos()) {

            String chaveExec = ag.getId() + "-" + hora + ":" + minuto;

            // Evita repetição no mesmo minuto
            if (executadosHoje.contains(chaveExec))
                continue;

            if (ag.getHora() == hora && ag.getMinuto() == minuto) {

                System.out.println("⏰ Executando agendamento das "
                        + ag.getHora() + ":" + ag.getMinuto());

                // 1 — Mandar comando para ESP
                try {
                    RestTemplate rest = new RestTemplate();
                    String url = "http://192.168.2.117/alimentar?quantidade=" + ag.getQuantidade();

                    rest.getForObject(url, String.class);

                    // 2 — Registrar histórico
                    historicoService.salvar(
                            new com.example.historico_auth_api.domain.Historico(
                                    null,
                                    String.valueOf(ag.getQuantidade()),
                                    LocalDateTime.now()));

                } catch (Exception e) {
                    System.out.println("Erro ao comunicar com ESP: " + e.getMessage());
                }

                // Marca como executado neste minuto
                executadosHoje.add(chaveExec);
            }
        }
    }
}
