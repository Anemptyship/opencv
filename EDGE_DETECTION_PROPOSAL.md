# 새로운 엣지 검출 알고리즘 제안: Phase Congruency 기반 엣지 검출

## 개요
Phase Congruency는 이미지의 여러 스케일에서 위상이 일치하는 지점을 엣지로 판단하는 수학적으로 우아한 방법입니다.

## 수학적 배경

### Phase Congruency 정의
```
PC(x,y) = Σ_n W_n(x,y) [A_n(x,y)ΔΦ_n(x,y) - T] / (ε + Σ_n A_n(x,y))
```

여기서:
- `A_n(x,y)`: n번째 스케일에서의 진폭
- `ΔΦ_n(x,y)`: 위상 편차
- `W_n(x,y)`: 가중치 함수
- `T`: 노이즈 임계값
- `ε`: 작은 상수 (0으로 나누기 방지)

### 구현 단계

1. **멀티스케일 Gabor 필터 뱅크 생성**
   - 여러 스케일과 방향의 Gabor 필터 생성
   - 일반적으로 4-6개 스케일, 6-8개 방향

2. **위상 및 진폭 계산**
   - 각 스케일/방향에서 위상과 진폭 추출
   - FFT 또는 Gabor 필터 컨볼루션 사용

3. **Phase Congruency 계산**
   - 각 픽셀에서 위상 일치성 측정
   - 가중치 적용하여 노이즈 억제

4. **엣지 맵 생성**
   - Phase Congruency 값에 임계값 적용
   - Non-maximum suppression (선택적)

## 장점

1. **밝기 변화에 강건**: 위상은 밝기 변화에 덜 민감
2. **얇은 엣지 보존**: 기존 방법보다 얇은 엣지도 잘 검출
3. **노이즈 강건성**: 여러 스케일 정보를 결합하여 노이즈 억제
4. **방향 정보 제공**: 각 픽셀의 엣지 방향도 함께 제공 가능

## 구현 파일 구조

```
modules/imgproc/src/
  - phase_congruency.cpp      # 메인 구현
  - phase_congruency.hpp       # 헤더 (선택적)
  
modules/imgproc/include/opencv2/imgproc.hpp
  - CV_EXPORTS_W void phaseCongruencyEdges(...) 선언 추가

modules/imgproc/test/
  - test_phase_congruency.cpp # 테스트 코드
```

## API 제안

```cpp
/** @brief Detects edges using Phase Congruency method.

The function detects edges in an image using Phase Congruency, which is 
robust to illumination changes and noise.

@param src Input single-channel 8-bit or floating-point image.
@param dst Output edge map; single-channel 8-bit image.
@param numScales Number of scales for Gabor filter bank (default: 4).
@param numOrientations Number of orientations for Gabor filter bank (default: 6).
@param threshold Threshold for edge detection (default: 0.1).
@param noiseThreshold Noise threshold T in the formula (default: 0.01).
@param k Noise compensation factor (default: 2.0).
@param sigma Standard deviation of Gaussian envelope in Gabor filters (default: 0.5).
@param minWaveLength Minimum wavelength in pixels (default: 3).
@param mult Multiplicative factor between scales (default: 2.1).
@param orientationEdges Optional output for orientation of edges (CV_32FC1).
*/
CV_EXPORTS_W void phaseCongruencyEdges(
    InputArray src,
    OutputArray dst,
    int numScales = 4,
    int numOrientations = 6,
    double threshold = 0.1,
    double noiseThreshold = 0.01,
    double k = 2.0,
    double sigma = 0.5,
    double minWaveLength = 3.0,
    double mult = 2.1,
    OutputArray orientationEdges = noArray()
);
```

## 성능 최적화 방안

1. **SIMD 최적화**: Gabor 필터 컨볼루션을 AVX2/AVX-512로 최적화
2. **병렬 처리**: 각 스케일/방향을 병렬로 처리
3. **메모리 최적화**: 중간 결과 재사용

## 참고 문헌

- Kovesi, P. (1999). "Image Features From Phase Congruency"
- Kovesi, P. (2003). "Phase Congruency Detects Corners and Edges"
