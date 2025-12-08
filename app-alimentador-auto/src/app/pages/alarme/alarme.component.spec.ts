import { ComponentFixture, TestBed } from '@angular/core/testing';

import { AlarmeComponent } from './alarme.component';

describe('AlarmeComponent', () => {
  let component: AlarmeComponent;
  let fixture: ComponentFixture<AlarmeComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      imports: [AlarmeComponent]
    })
    .compileComponents();

    fixture = TestBed.createComponent(AlarmeComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
