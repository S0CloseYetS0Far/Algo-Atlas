class Solution:
    def buildBridge(self, _num: int, wood: List[List[int]]) -> int:
        S = SlopeTrick()
        length = [b - a for a, b in wood]
        for i, (left, _) in enumerate(wood):
            S.addLeftOffset(-length[i])
            if i > 0:
                S.addRightOffset(length[i - 1]) 
            S.addAbsXMinusA(left)
        return S.getMinY()



INF = int(1e18)

class SlopeTrick:
    """
    https://maspypy.com/slope-trick-1-%e8%a7%a3%e8%aa%ac%e7%b7%a8

    A template implemented by @caomeinaixi, based on the article above.
    """

    __slots__ = "_minY", "_leftTuring", "_rightTuring", "_leftOffset", "_rightOffset"

    def __init__(
        self, leftTuring: Optional[List[int]] = None, rightTuring: Optional[List[int]] = None
    ) -> None:
        self._minY = 0  # minimum dp value
        self._leftTuring = [INF] if leftTuring is None else leftTuring  # left turning points
        self._rightTuring = [INF] if rightTuring is None else rightTuring  # right turning points
        self._leftOffset = 0  # shift amount of the left turning points
        self._rightOffset = 0  # shift amount of the right turning points

    def addAbsXMinusA(self, a: int) -> None:
        """Add |x-a|: O(logn) time"""
        self.addXMinusA(a)
        self.addAMinusX(a)

    def addXMinusA(self, a: int) -> None:
        """Add (x-a)+: O(logn) time

        a is added to the slope change points
        the change in minY equals f(left0)
        """
        if len(self._leftTuring) != 0:
            self._minY += max(0, self.leftTop - a)
        self._pushLeft(a)
        self._pushRight(self._popLeft())

    def addAMinusX(self, a: int) -> None:
        """Add (a-x)+: O(logn) time

        a is added to the slope change points
        the change in minY equals f(right0)
        """
        if len(self._rightTuring) != 0:
            self._minY += max(0, a - self.rightTop)
        self._pushRight(a)
        self._pushLeft(self._popRight())

    def addY(self, delta: int) -> None:
        """Add y: O(1) time"""
        self._minY += delta

    def addOffset(self, delta: int) -> None:
        """Shift: O(1) time

        g(x) = f(x - a)
        replace f with g
        """
        self._leftOffset += delta
        self._rightOffset += delta

    def addLeftOffset(self, delta: int) -> None:
        """Shift the left turning points: O(1) time"""
        self._leftOffset += delta

    def addRightOffset(self, delta: int) -> None:
        """Shift the right turning points: O(1) time"""
        self._rightOffset += delta

    def updateLeftMin(self) -> None:
        """Cumulative min: O(1) time

        g(x) = min(f(y) | y <= x)
        replace f with g

        replace rightTuring with the empty set
        """
        self._rightTuring = [INF]

    def updateRightMin(self) -> None:
        """Cumulative min: O(1) time

        g(x) = min(f(y) | y >= x)
        replace f with g

        replace leftTuring with the empty set
        """
        self._leftTuring = [INF]

    def updateWindowMin(self, leftDiff: int, rightDiff: int) -> None:
        """Cumulative min: O(1) time

        g(x) = min(f(y) | `x - leftDiff <= y <= x - rightDiff`)
        replace f with g

        shift the left set and the right set respectively
        left0, right0 => left0 + rightDiff, right0 + leftDiff
        """
        self._leftOffset += rightDiff
        self._rightOffset += leftDiff

    def getMinY(self) -> int:
        """Get the minimum value: O(1) time"""
        return self._minY

    def _pushLeft(self, a: int) -> None:
        heappush(self._leftTuring, -a + self._leftOffset)

    def _pushRight(self, a: int) -> None:
        heappush(self._rightTuring, a - self._rightOffset)

    def _popLeft(self) -> int:
        return -heappop(self._leftTuring) + self._leftOffset

    def _popRight(self) -> int:
        return heappop(self._rightTuring) + self._rightOffset

    @property
    def leftTop(self) -> int:
        """Get left0, the maximum slope change point on the left side: O(1) time"""
        return -self._leftTuring[0] + self._leftOffset

    @property
    def rightTop(self) -> int:
        """Get right0, the minimum slope change point on the right side: O(1) time"""
        return self._rightTuring[0] + self._rightOffset
