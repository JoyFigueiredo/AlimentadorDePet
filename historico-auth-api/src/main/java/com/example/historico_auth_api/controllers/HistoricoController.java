package com.example.historico_auth_api.controllers;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.*;

import com.example.historico_auth_api.domain.Historico;
import com.example.historico_auth_api.repositories.HistoricoRepository;

import java.time.LocalDate;
import java.time.LocalTime;
import java.util.List;

@RestController
@RequestMapping("/qnt")
@CrossOrigin(origins = "*") // permite o acesso do Angular
public class HistoricoController {

    @Autowired
    private HistoricoRepository repository;

    // ✅ Retorna todos os registros
    @GetMapping
    public List<Historico> listar() {
        return repository.findAll();
    }

    // ✅ Adiciona novo registro
    @PostMapping
    public Historico salvar(@RequestBody Historico novo) {
        // define data/hora atuais automaticamente
        novo.setData(LocalDate.now());
        novo.setHora(LocalTime.now());
        return repository.save(novo);
    }

    // ✅ Limpa o histórico
    @DeleteMapping("/resetar")
    public void resetar() {
        repository.deleteAll();
    }
}
