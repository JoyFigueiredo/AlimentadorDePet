package com.example.historico_auth_api.repositories;

import org.springframework.data.jpa.repository.JpaRepository;
import com.example.historico_auth_api.domain.Historico;

public interface HistoricoRepository extends JpaRepository<Historico, String> {
}
