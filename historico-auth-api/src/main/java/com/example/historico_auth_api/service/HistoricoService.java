package com.example.historico_auth_api.service;

import java.util.List;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import com.example.historico_auth_api.domain.Historico;
import com.example.historico_auth_api.repositories.HistoricoRepository;

@Service
public class HistoricoService {

    @Autowired
    private HistoricoRepository repository;

    public List<Historico> listarTodos() {
        return repository.findAll();
    }

    public Historico salvar(Historico historico) {
        return repository.save(historico);
    }

    public void deletarTodos() {
        repository.deleteAll();
    }
}
