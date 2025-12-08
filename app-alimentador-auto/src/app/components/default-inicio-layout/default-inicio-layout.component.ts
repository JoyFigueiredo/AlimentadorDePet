import { Component, EventEmitter, Output, Input } from '@angular/core';
import { ɵEmptyOutletComponent } from '@angular/router';

@Component({
  selector: 'app-default-inicio-layout',
  standalone: true,
  templateUrl: './default-inicio-layout.component.html',
  styleUrls: ['./default-inicio-layout.component.css'],
})
export class DefaultinicioLayoutComponent {
  @Input() title: string = '';
  @Input() primaryBtnText: string = '';
  @Input() secondaryBtnText: string = '';

  @Output() primaryBtnClick = new EventEmitter<void>();
  @Output() secondaryBtnClick = new EventEmitter<void>();

  onPrimaryClick() {
    this.primaryBtnClick.emit();
  }

  onSecondaryClick() {
    this.secondaryBtnClick.emit();
  }
}
