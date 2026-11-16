#include "fsm_automatic.h"
#include "traffic_light.h"
#include "led7seg.h"
#include "software_timer.h"
#include "button.h"

int count_dir1 = 0;
int count_dir2 = 0;

/**
 * @brief Nạp thời gian đếm ngược và hiển thị lên LED 7 đoạn.
 */
void setCountdown(int time_dir1, int time_dir2) {
	count_dir1 = time_dir1;
	count_dir2 = time_dir2;
	updateMode7SEG(count_dir1, count_dir2);
}

/**
 * @brief Đếm ngược 1 giây cho LED 7 đoạn (dùng timer[3]).
 */
void countdownTick(void) {
	count_dir1--;
	count_dir2--;
	updateMode7SEG(count_dir1, count_dir2);
	setTimer(3, 1000); // Nạp lại 1 giây cho timer[3]
}

/**
 * @brief Khởi tạo chế độ tự động ban đầu.
 */
void fsm_automatic_init(void) {
	status = STATUS_AUTO_RED_GREEN;
	setTraffic_RedGreen();
	setCountdown(red_time, green_time);

	setTimer(0, green_time * 1000); // timer[0]: Độ dài pha Đỏ - Xanh = green_time (ms)
	setTimer(3, 1000);              // timer[3]: Nhịp đếm 1 giây cho LED 7 đoạn
}

void fsm_automatic(void) {
	switch (status) {
	case STATUS_AUTO_RED_GREEN:
		// Mũi tên 1: Nút 1 nhấn -> Chuyển sang Manual (tại pha C1)
		if (isButtonPressed(0)) {
			status = STATUS_MAN_RED_GREEN;
			setTraffic_RedGreen();
			updateMode7SEG(1, 0);
			break;
		}
		// Mũi tên 2: HẾT THỜI GIAN PHA ĐỎ - XANH -> Chuyển sang Đỏ - Vàng
		if (timer_flag[0] == 1) {
			status = STATUS_AUTO_RED_YELLOW;
			setTraffic_RedYellow();
			setCountdown(amber_time, amber_time);
			setTimer(0, amber_time * 1000); // Pha Đỏ - Vàng kéo dài amber_time (ms)
			setTimer(3, 1000);              // Đồng bộ lại nhịp 1s cho 7SEG
			break;
		}
		// Mũi tên 3 (Self-loop): Nhịp đếm 1 giây hiển thị LED 7 đoạn
		if (timer_flag[3] == 1) {
			countdownTick();
			break;
		}
		break;

	case STATUS_AUTO_RED_YELLOW:
		// Mũi tên 1: HẾT THỜI GIAN PHA ĐỎ - VÀNG -> Chuyển sang Xanh - Đỏ
		if (timer_flag[0] == 1) {
			status = STATUS_AUTO_GREEN_RED;
			setTraffic_GreenRed();
			setCountdown(green_time, red_time);
			setTimer(0, green_time * 1000); // Pha Xanh - Đỏ kéo dài green_time (ms)
			setTimer(3, 1000);
			break;
		}
		// Mũi tên 2 (Self-loop): Nhịp đếm 1 giây hiển thị LED 7 đoạn
		if (timer_flag[3] == 1) {
			countdownTick();
			break;
		}
		break;

	case STATUS_AUTO_GREEN_RED:
		// Mũi tên 1: HẾT THỜI GIAN PHA XANH - ĐỎ -> Chuyển sang Vàng - Đỏ
		if (timer_flag[0] == 1) {
			status = STATUS_AUTO_YELLOW_RED;
			setTraffic_YellowRed();
			setCountdown(amber_time, amber_time);
			setTimer(0, amber_time * 1000); // Pha Vàng - Đỏ kéo dài amber_time (ms)
			setTimer(3, 1000);
			break;
		}
		// Mũi tên 2 (Self-loop): Nhịp đếm 1 giây hiển thị LED 7 đoạn
		if (timer_flag[3] == 1) {
			countdownTick();
			break;
		}
		break;

	case STATUS_AUTO_YELLOW_RED:
		// Mũi tên 1: HẾT THỜI GIAN PHA VÀNG - ĐỎ -> Quay về Đỏ - Xanh
		if (timer_flag[0] == 1) {
			status = STATUS_AUTO_RED_GREEN;
			setTraffic_RedGreen();
			setCountdown(red_time, green_time);
			setTimer(0, green_time * 1000); // Pha Đỏ - Xanh kéo dài green_time (ms)
			setTimer(3, 1000);
			break;
		}
		// Mũi tên 2 (Self-loop): Nhịp đếm 1 giây hiển thị LED 7 đoạn
		if (timer_flag[3] == 1) {
			countdownTick();
			break;
		}
		break;

	default:
		break;
	}
}
