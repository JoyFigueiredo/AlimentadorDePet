import { ComponentFixture, TestBed } from '@angular/core/testing';

import { DefaultinicioLayoutComponent } from './default-inicio-layout.component';

describe('DefaultloginLayoutComponent', () => {
  let component: DefaultinicioLayoutComponent;
  let fixture: ComponentFixture<DefaultinicioLayoutComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      imports: [DefaultinicioLayoutComponent],
    }).compileComponents();

    fixture = TestBed.createComponent(DefaultinicioLayoutComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
