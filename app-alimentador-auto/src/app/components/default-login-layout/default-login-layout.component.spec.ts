import { ComponentFixture, TestBed } from '@angular/core/testing';

import { DefaultloginLayoutComponent } from './default-login-layout.component';

describe('DefaultloginLayoutComponent', () => {
  let component: DefaultloginLayoutComponent;
  let fixture: ComponentFixture<DefaultloginLayoutComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      imports: [DefaultloginLayoutComponent]
    })
    .compileComponents();

    fixture = TestBed.createComponent(DefaultloginLayoutComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
