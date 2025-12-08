package com.example.historico_auth_api.service;

import com.example.historico_auth_api.domain.Agendamento;
import com.example.historico_auth_api.repositories.AgendamentoRepository;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import java.util.List;

@Service
public class AgendamentoService {

    @Autowired
    private AgendamentoRepository repository;

    public List<Agendamento> listarTodos() {
        return repository.findAll();
    }

    public Agendamento salvar(Agendamento agendamento) {
        return repository.save(agendamento);
    }

    public void deletar(String id) {
        repository.deleteById(id);
    }
}
