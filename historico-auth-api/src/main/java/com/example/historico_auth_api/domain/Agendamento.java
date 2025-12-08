package com.example.historico_auth_api.domain;

import jakarta.persistence.*;
import lombok.*;

@Entity
@Table(name = "agendamento")
@Getter
@Setter
@AllArgsConstructor
@NoArgsConstructor
public class Agendamento {

    @Id
    @GeneratedValue(strategy = GenerationType.UUID)
    private String id;

    private int hora;
    private int minuto;
    private double quantidade;
}
