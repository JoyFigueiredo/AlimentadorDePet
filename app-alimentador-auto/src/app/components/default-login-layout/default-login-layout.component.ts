import { Component, Input } from '@angular/core';
import { ɵEmptyOutletComponent } from "@angular/router";

@Component({
  selector: 'app-default-login-layout',
  imports: [ɵEmptyOutletComponent],
  templateUrl: './default-login-layout.component.html',
  styleUrl: './default-login-layout.component.css'
})
export class DefaultLoginLayoutComponent {
  @Input() title: string = "";
  @Input() primaryBtnText: string = "";
  @Input() secondaryBtnText: string = "";

  @Output() primaryBtnClick = new EventEmitter<void>();

  onPrimaryClick() {
    this.primaryBtnClick.emit();
  }
}



