package com.example.historico_auth_api.controllers;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;
import com.example.historico_auth_api.domain.Historico;
import com.example.historico_auth_api.service.HistoricoService;

import java.util.List;

@RestController
@RequestMapping("/historico")
@CrossOrigin(origins = "*")
public class HistoricoController {

    @Autowired
    private HistoricoService service;

    // Retorna histórico
    @GetMapping
    public List<Historico> listarHistorico() {
        return service.listarTodos();
    }

    // Reseta histórico
    @DeleteMapping
    public void resetarHistorico() {
        service.deletarTodos();
    }

    @PostMapping
    public Historico alimentar(@RequestBody Historico historico) {
        historico.setDataHora(java.time.LocalDateTime.now());
        return service.salvar(historico);
    }
}
